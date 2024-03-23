/*--------------------------------------------------------------------*/
/* symtablelist.c                                                       */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

/* Each item is stored in a stNode.  stNodes are linked to form a list.*/
struct stNode {
   /* pointer pointing to data. */
   const void *pvValue;
   /* unique key pointer*/
   char *pcKey;
   /* The pointer to next stNode. */
   struct stNode *psNextNode;
};


/* A SymTable "dummy" node to point to the first stNode. */
struct SymTable {
   /* The pointer to the first stNode. */
   struct stNode *psFirstNode;
   /* length of SymTable*/
   size_t length; 
};

 SymTable_T SymTable_new(void){
   SymTable_T oSymtable;

   oSymtable = (SymTable_T)malloc(sizeof(struct SymTable));
   if (oSymtable == NULL)
      return NULL;
   oSymtable->length = 0;
   oSymtable->psFirstNode = NULL;
   return oSymtable;
}

void SymTable_free(SymTable_T oSymTable) {
   struct stNode *psCurrentNode;
   struct stNode *psNextNode;

   assert(oSymTable != NULL);

   for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psNextNode)
   {
        psNextNode = psCurrentNode->psNextNode;
        free(psCurrentNode->pcKey);
        free(psCurrentNode);
   }

   free(oSymTable);
}


size_t SymTable_getLength(SymTable_T oSymTable){
    assert(oSymTable != NULL);
    return oSymTable->length;
}


int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue){
    struct stNode *psNewNode;
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
    strcpy((char*)psNewNode->pcKey,pcKey);

    psNewNode->pvValue = pvValue;
    psNewNode->psNextNode = oSymTable->psFirstNode;
    oSymTable->psFirstNode = psNewNode;
    oSymTable->length++;
    return 1;
}


void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue){ 
    struct stNode *psCurrentNode;
    const void* oldValue; 

    assert(oSymTable != NULL);
    assert(pcKey != NULL);


    for(psCurrentNode = oSymTable->psFirstNode;
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for(psCurrentNode = oSymTable->psFirstNode;
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

    assert(oSymTable != NULL);
    assert(pcKey != NULL);

    for(psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode){
            if(strcmp((char *) psCurrentNode->pcKey, pcKey) == 0){
                return (void*) psCurrentNode->pvValue;
            }
        }  
    return NULL;
  }


void *SymTable_remove(SymTable_T oSymTable, const char *pcKey){
    struct stNode *deleteNode;
    struct stNode *newNextNode; 
    struct stNode *previousNode;
    const void* oldValue; 

    assert(oSymTable != NULL);
    assert(pcKey != NULL);
    if(oSymTable->psFirstNode == NULL){
        return NULL;
    }

    /*if you want to remove the first node...*/
    if(strcmp((char *) oSymTable->psFirstNode->pcKey, pcKey) == 0){
        oldValue = oSymTable->psFirstNode->pvValue;
        deleteNode = oSymTable->psFirstNode;
        newNextNode = deleteNode->psNextNode;
        oSymTable->psFirstNode = newNextNode;
        free(deleteNode->pcKey);
        free(deleteNode);
        oSymTable->length--;
        return (void*)oldValue;
    }
    for(deleteNode = oSymTable->psFirstNode;
        deleteNode->psNextNode != NULL;
        deleteNode = deleteNode->psNextNode){
            if(strcmp((char *) deleteNode->psNextNode->pcKey, pcKey) == 0){
                previousNode = deleteNode; 
                deleteNode = deleteNode->psNextNode;
                oldValue = deleteNode->pvValue;
                newNextNode = deleteNode->psNextNode;
                previousNode->psNextNode = newNextNode;
                free(deleteNode->pcKey);
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

    assert(oSymTable != NULL);
    assert(pfApply != NULL);


    for (psCurrentNode = oSymTable->psFirstNode;
        psCurrentNode != NULL;
        psCurrentNode = psCurrentNode->psNextNode) 
            (*pfApply)((void*)psCurrentNode->pcKey,(void*)psCurrentNode->pvValue, (void*)pvExtra);
}