/*-------------------------------------------------------------------*/
/* symtablelist.c                                                    */
/* Author: Aidan Walsh                                               */
/*-------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

struct SymTable 
{
    size_t count; 
    struct Node *first; 

};

struct Node 
{
 char *Key; 
 const void *Value; 
 struct Node *next; 
};

SymTable_T SymTable_new(void)
{
    SymTable_T oSymTable; 
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable)); 
    if (oSymTable == NULL)
        return NULL; 
    
    oSymTable->first = NULL; 
    oSymTable->count = 0; 
    return oSymTable; 

}

void SymTable_free(SymTable_T oSymTable)
{
    struct Node *currentNode; 
    struct Node *nextNode; 

   assert(oSymTable != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        free((void*)currentNode->Key);
        free(currentNode); 
    }
    free(oSymTable); 
}

size_t SymTable_getLength(SymTable_T oSymTable)
{
    return oSymTable->count; 
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
 const void *pvValue)
 {
  
    struct Node *currentNode; 
    struct Node *nextNode; 
    struct Node *newNode; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 
    /* assert(pvValue != NULL); */

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        if (*(currentNode->Key) == *pcKey) {
            return 0; 
        }
    }
    newNode = (struct Node*)malloc(sizeof(struct Node)); 
            if (newNode == NULL) {
                return 0; 
            }
            newNode->Value = pvValue; 
            newNode->next = oSymTable->first; 
            oSymTable->first = newNode; 
            newNode->Key = (char*)malloc((strlen(pcKey)+1)*sizeof(char));
            if(newNode->Key == NULL) {
                return 0; 
            }
            newNode->Key = strcpy(newNode->Key, pcKey);
           (oSymTable->count)++; 
            return 1; 
    


 }

 void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue)
{
    struct Node *currentNode; 
    struct Node *nextNode; 
     void *returned; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 
    /* assert(pvValue != NULL); */

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             returned = (void*) (currentNode->Value); 
            currentNode->Value = pvValue; 
            return returned; 
        }
    }
       
    return NULL; 


}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) 
{
     struct Node *currentNode; 
    struct Node *nextNode; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             return 1; 
         }
    }
    return 0; 

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey)
{
    struct Node *currentNode; 
    struct Node *nextNode; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             return (void*) currentNode->Value; 
         }
    }
    return NULL; 

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {
    struct Node *currentNode; 
    struct Node *nextNode; 
    struct Node *previousNode; 
    void *returned; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 
   
    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        
        nextNode = currentNode->next; 
       
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
            
             (oSymTable->count)--;
             returned = (void*) currentNode->Value; 

             /* corner case for only 1 node*/
             if(oSymTable->first == currentNode && nextNode == NULL)
             {
                 oSymTable->first = nextNode; 
                 free(currentNode->Key);
                 free(currentNode); 
                 oSymTable->first = NULL; 
                 return returned; 
             }

             /* corner case for at end */
            
             if(nextNode == NULL) {
                 
                 free(currentNode->Key);
                 free(currentNode); 
                 return returned; 
             }

            /* corner case for at front */
             if(oSymTable->first == currentNode)
             {
                 oSymTable->first = nextNode; 
                 free(currentNode->Key);
                 free(currentNode); 
                 oSymTable->first = NULL; 
                 return returned; 
             }

             
             /* normal case for in middle */

             previousNode->next = nextNode; 
             free(currentNode->Key);
             free(currentNode); 
             return returned; 



             
         }
         previousNode = currentNode; 
    }
    return NULL; 

}

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra)
{
     struct Node *currentNode; 
    struct Node *nextNode; 

     assert(oSymTable != NULL); 
     assert(pfApply != NULL); 

     for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        (*pfApply)(currentNode->Key, (void*) currentNode->Value,
         (void*) pvExtra);
    }
}

