/*-------------------------------------------------------------------*/
/* symtable.h                                                        */
/* Author: Aidan Walsh                                               */
/*-------------------------------------------------------------------*/

#ifndef symtable_INCLUDED
#define symtable_INCLUDED
#include <stddef.h>


/* pointer to ADT SymTable_T that is defined in the implementations.
When creating new SymTable, client is responsible for freeing using
SymTable_free(SymTable_T oSymTable) */
typedef struct SymTable *SymTable_T; 

/*return a new SymTable objet that contains no bindings, or NULL if
insufficient memory is available. When creating new SymTable, client 
is responsible for freeing using SymTable_free(SymTable_T oSymTable)
 */
SymTable_T SymTable_new(void);

/* frees all memory occupied by oSymTable */
void SymTable_free(SymTable_T oSymTable);

/* return the number of bindings in oSymTable. Assert that oSymTable
is not NULL */ 
size_t SymTable_getLength(SymTable_T oSymTable); 

/* If oSymTable does not contain a binding with key pcKey, then S
ymTable_put must add a new binding to oSymTable consisting of key 
pcKey and value pvValue and return 1 (TRUE). Otherwise the function
 must leave oSymTable unchanged and return 0 (FALSE). If 
 insufficient memory is available, then the function must 
 leave oSymTable unchanged and return 0 (FALSE). Assert that 
 oSymTable is not NULL  */
int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue);

/* If oSymTable contains a binding with key pcKey, then 
SymTable_replace must replace the binding's value with pvValue
 and return the old value. Otherwise it must leave oSymTable 
 unchanged and return NULL. Assert that all arguments except
 pvValue are not NULL. */
void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue);

/* SymTable_contains must return 1 (TRUE) if oSymTable contains
 a binding whose key is pcKey, and 0 (FALSE) otherwise. Assert
 that all arguments are not NULL. */
int SymTable_contains(SymTable_T oSymTable, const char *pcKey);

/* SymTable_get must return the value of the binding within 
oSymTable whose key is pcKey, or NULL if no such binding 
exists. Assert that all arguments are not NULL.*/
void *SymTable_get(SymTable_T oSymTable, const char *pcKey);

/* If oSymTable contains a binding with key pcKey, then 
SymTable_remove must remove that binding from oSymTable and 
return the binding's value. Otherwise the function must not 
change oSymTable and return NULL. Assert that all arguments are 
not NULL. */
void *SymTable_remove(SymTable_T oSymTable, const char *pcKey);

/* SymTable_map must apply function *pfApply to each binding in
 oSymTable, passing pvExtra as an extra parameter. That is, 
 the function must call (*pfApply)(pcKey, pvValue, pvExtra) 
 for each pcKey/pvValue binding in oSymTable. Assert that 
 oSymTable and pfApply are not NULL. */
void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra);
  


#endif