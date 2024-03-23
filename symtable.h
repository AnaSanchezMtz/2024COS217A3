/*--------------------------------------------------------------------*/
/* symtable.h                                                         */
/*--------------------------------------------------------------------*/

#ifndef SYMTABLE_INCLUDED
#define SYMTABLE_INCLUDED
#include <stddef.h>

/*--------------------------------------------------------------------*/

/*Return a new SymTable object with bindings containing key-value pairs,
    where the key is the string and the value is a void pointer referencing
    a data of any type, or NULL if insufficent memory is avaible */
typedef struct  SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/*Return a new SymTable object, or NULL if insufficent memory is avaible */
SymTable_T SymTable_new(void);

/*--------------------------------------------------------------------*/
/*Free all memory in oSymTable*/
void SymTable_free(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/
/*Return number of bindings in oSymTable*/
size_t SymTable_getLength(SymTable_T oSymTable);

/*--------------------------------------------------------------------*/
/*If oSymtable DOES NOT contain contain binding with pcKey, put new 
    binding consisting of pcKey and pcValue and return 1(true). Otherwise,
    leave oSymtable unchanged and return 0. Return 0 if insufficient memory.*/
int SymTable_put(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/
/*If oSymtable contains binding with pcKey, replace that bindings value
    with pcValue and return old value. Otherwise, leave oSymtable unchanged
    and return NULL*/
void *SymTable_replace(SymTable_T oSymTable,
    const char *pcKey, const void *pvValue);

/*--------------------------------------------------------------------*/
/*Return 1(TRUE) if oSymtable contains binding with pcKey, and 0(FALSE)
    otherwise*/
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/*Return value of binding within pSymtable whose key is pcKey or NULL
    if no such binding exists*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/*If oSymTable contains binding with pcKey, remove that binding and 
    return that binding's value. Otherwise, leave oSymtable unchanged 
    and return NULL*/
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/*--------------------------------------------------------------------*/
/*Call (*pfApply)(pcKey, pvValue, pvExtra) for each pcKey/pvValue binding 
    in oSymTable.*/
void SymTable_map(SymTable_T oSymTable,
     void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
     const void *pvExtra);

#endif