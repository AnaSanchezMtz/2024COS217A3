/*--------------------------------------------------------------------*/
/* symtablehash.c                                                     */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

/* global variables*/
/* # of buckets in hashtable */
static const size_t auBucketCounts[] = {509, 1021}; 

/* Each item is stored in a stNode.  stNodes are linked to form a list.  */
struct stNode {
   /* pointer pointing to data. */
   const void *pvValue;
   /* unique key pointer*/
   const char *pcKey;
   /* The address of the next stNode. */
   struct stNode *psNextNode;
};


/* A SymTable "dummy" node to point to the first stNode. */
struct SymTable {
   /* The address of the array of Nodes */
   /* Second * makes it an array of pointers */
   struct stNode **psFirstNode; 
   /* Number of Nodes in SymTable*/
   size_t length; 
   /*number of addresses in SymTable*/
   size_t auBucketCounts; 
};


static size_t SymTable_hash(const char *pcKey, size_t uBucketCount)
{
   const size_t HASH_MULTIPLIER = 65599;
   size_t u;
   size_t uHash = 0;

   assert(pcKey != NULL);

   for (u = 0; pcKey[u] != '\0'; u++)
      uHash = uHash * HASH_MULTIPLIER + (size_t)pcKey[u];

   return uHash % uBucketCount;
}

 SymTable_T SymTable_new(void){
    size_t i;
    SymTable_T oSymtable;
    size_t numberofBuckets = 509; 
    oSymtable = (SymTable_T)malloc(sizeof(struct SymTable));
    if (oSymtable == NULL)
        return NULL;
    
    oSymtable->psFirstNode = 
        (struct stNode**)malloc(sizeof(struct stNode*)*numberofBuckets);
    if (oSymtable->psFirstNode == NULL){
    free(oSymtable);
    return NULL;
    }

    oSymtable->length = 0;
    oSymtable->auBucketCounts = auBucketCounts[0];
    for(i = 0; i < numberofBuckets; i++){
        oSymtable->psFirstNode[i] = NULL;   
    }
  
    return oSymtable;
}

void SymTable_free(SymTable_T oSymTable) {
    struct stNode *psCurrentNode;
    struct stNode *psNextNode;
    size_t i;

   assert(oSymTable != NULL);

    for(i = 0; i < oSymTable->auBucketCounts; i++){
        for (psCurrentNode = oSymTable->psFirstNode[i];
            psCurrentNode != NULL;
            psCurrentNode = psNextNode)
        {
            psNextNode = psCurrentNode->psNextNode;
            free((char *)psCurrentNode->pcKey);
            free(psCurrentNode);
        }
    }
    free(oSymTable->psFirstNode);
    free(oSymTable);
}

size_t SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable != NULL);
    return oSymTable->length;
  }

int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue){
    struct stNode *psNewNode;
    size_t hashNumber;
    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    if(SymTable_contains(oSymTable,pcKey)== 1) 
    {
        return 0;
    }

    psNewNode = (struct stNode*)malloc(sizeof(struct stNode));
    if (psNewNode == NULL)
        return 0;
    
    psNewNode->pcKey = (char*)malloc(strlen(pcKey) + 1);
    if(psNewNode->pcKey == NULL){
        free(psNewNode);
        return 0; 
    }

    /*make defensive copy*/
    strcpy((char*)psNewNode->pcKey,pcKey);
    
    hashNumber = SymTable_hash(pcKey, oSymTable->auBucketCounts);
    
    psNewNode->pvValue = pvValue;
    
    psNewNode->psNextNode = oSymTable->psFirstNode[hashNumber];
    oSymTable->psFirstNode[hashNumber] = psNewNode;
    oSymTable->length++;
    return 1;
}

void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue){ 
    struct stNode *psCurrentNode;
    const void* oldValue; 
    size_t hashNumber;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashNumber = SymTable_hash(pcKey, oSymTable->auBucketCounts);

    for(psCurrentNode = oSymTable->psFirstNode[hashNumber];
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode){
            if(strcmp((char *) psCurrentNode->pcKey, pcKey) == 0){
                oldValue = psCurrentNode->pvValue;
                psCurrentNode->pvValue = pvValue;
                return (void*) oldValue; 
                }
        }
    return NULL;
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey){
    struct stNode *psCurrentNode;
    size_t hashNumber;


    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashNumber = SymTable_hash(pcKey, oSymTable->auBucketCounts);

    for(psCurrentNode = oSymTable->psFirstNode[hashNumber];
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode){
            if(strcmp((char *) psCurrentNode->pcKey, pcKey) == 0){
                return 1;
            }
        }  
    return 0;
  }

  void *SymTable_get(SymTable_T oSymTable, const char *pcKey){
    struct stNode *psCurrentNode;
    size_t hashNumber;

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    hashNumber = SymTable_hash(pcKey, oSymTable->auBucketCounts);

    for(psCurrentNode = oSymTable->psFirstNode[hashNumber];
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode){
            if(strcmp((char *) psCurrentNode->pcKey, pcKey) == 0){
                return (void*) psCurrentNode->pvValue;
            }
        }  
    return NULL;
  }

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    size_t hashNumber;
    struct stNode *deleteNode;
    struct stNode *newNextNode; 
    struct stNode *previousNode;
    const void* oldValue; 

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    hashNumber = SymTable_hash(pcKey, oSymTable->auBucketCounts);
    if(oSymTable->psFirstNode[hashNumber] == NULL){
        return NULL;
    }

    /*if you want to remove the first node...*/
    if(strcmp((char *) oSymTable->psFirstNode[hashNumber]->pcKey, pcKey) == 0){
        oldValue = oSymTable->psFirstNode[hashNumber]->pvValue;
        deleteNode = oSymTable->psFirstNode[hashNumber];
        newNextNode = deleteNode->psNextNode;
        oSymTable->psFirstNode[hashNumber] = newNextNode;
        free((char *)deleteNode->pcKey);
        free(deleteNode);
        oSymTable->length--;
        return (void*)oldValue;
    }
    for(deleteNode = oSymTable->psFirstNode[hashNumber];
        deleteNode->psNextNode != NULL;
        deleteNode = deleteNode->psNextNode){
            if(strcmp((char *) deleteNode->psNextNode->pcKey, pcKey) == 0){
                previousNode = deleteNode; 
                deleteNode = deleteNode->psNextNode;
                oldValue = deleteNode->pvValue;
                newNextNode = deleteNode->psNextNode;
                previousNode->psNextNode = newNextNode;
                free((char *)deleteNode->pcKey);
                free(deleteNode);
                oSymTable->length--;
                return (void*)oldValue;
            }
        }
    return NULL;
}

void SymTable_map(SymTable_T oSymTable,
    void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
    const void *pvExtra) {
    struct stNode *psCurrentNode;
    size_t i;

    assert(oSymTable != NULL);
    assert(pfApply != NULL);


    for(i = 0; i < oSymTable->auBucketCounts; i++){
        for (psCurrentNode = oSymTable->psFirstNode[i];
            psCurrentNode != NULL;
            psCurrentNode = psCurrentNode->psNextNode){
                (*pfApply)((void*)psCurrentNode->pcKey,
                (void*)psCurrentNode->pvValue, (void*)pvExtra);
            } 
    }
}