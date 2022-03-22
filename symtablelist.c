/*-------------------------------------------------------------------*/
/* symtablelist.c                                                    */
/* Author: Aidan Walsh                                               */
/*-------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

/* structure that describes SymTable_T */
struct SymTable 
{
    /* need to keep track of count of all bindings we add */
    size_t count; 

    /* have to keep pointer that points to first node in the list */
    struct Node *first; 

};

/* structure that describes each node and what it stores */
struct Node 
{

/* each node must have an array of chars that are the key */
 char *Key; 

 /* void pointer to the value so that any type can be stored */
 const void *Value; 

 /* pointer to node next that is the node that follows the current */
 struct Node *next; 
};

SymTable_T SymTable_new(void)
{
    /* allocate memory for SymTable structure, then make sure memory 
    has been allocated properly and adjust initial values */
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
    /* create nodes to use for traversal */
    struct Node *currentNode; 
    struct Node *nextNode; 

   assert(oSymTable != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        /* first, free the key, then free the current node and 
        move on */
        nextNode = currentNode->next; 
        free((void*)currentNode->Key);
        free(currentNode); 
    }

    /* finally, free symbol table */
    free(oSymTable); 
}

size_t SymTable_getLength(SymTable_T oSymTable)
{
    return oSymTable->count; 
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey,
 const void *pvValue)
 {
  
  /* create new nodes for traversal then the newNode that will
  be the newNode created and stored */
    struct Node *currentNode; 
    struct Node *nextNode; 
    struct Node *newNode; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* make sure key is not equal, if equal, return 0 */
        if (strcmp(pcKey, currentNode->Key) == 0) {
            return 0; 
        }
    }

    /* since we have not found it, we allocate new space 
    and make sure it is properly allocated */
    newNode = (struct Node*)malloc(sizeof(struct Node)); 
            if (newNode == NULL) {
                return 0; 
            }

            /* adjust value, next, and make new node the 
            first node. Then allocate memory for the 
            key */
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
    /* create new nodes for traversal and void pointer that stores
    what should be returned */
    struct Node *currentNode; 
    struct Node *nextNode; 
     void *returned; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* if we find binding with key, then adjust
        returned value and return it */
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
    /* nodes for traversal */
     struct Node *currentNode; 
    struct Node *nextNode; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {

        /* if we find key, then we just return 1 since we have 
        found it */
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

    /* nodes for traversal */
    struct Node *currentNode; 
    struct Node *nextNode; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* if we find the key in a node then 
        we just return its value but have to cast
        since its return value is void* */
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             return (void*) currentNode->Value; 
         }
    }
    return NULL; 

}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {

    /* top two nodes for traversal. Previous node keeps track of the
    previous node we are on so if we are removing something in the middle
    of the list, we can have the previous node point to the next node.*/
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

        /* if we find the Key, then we remove*/
        if (strcmp(currentNode->Key, pcKey) == 0) {
             (oSymTable->count)--;
             returned = (void*) currentNode->Value; 
             /* corner case for only 1 node present, so we have to set 
             the first Node to NULL*/
             if(oSymTable->first == currentNode && nextNode == NULL) {
                 oSymTable->first = nextNode; 
                 free(currentNode->Key);
                 free(currentNode); 
                 oSymTable->first = NULL; 
                 return returned; 
                 }
             /* corner case for at end so our previous node's "next" 
              is NULL since we just removed our current and there 
              is nothing after it*/
             if(nextNode == NULL) {
                 free(currentNode->Key);
                 free(currentNode); 
                 previousNode->next = NULL;  
                 return returned; 
                 }
            /* corner case for at front so we adjust the first
            node of it all to be the next node. Everything still
            points to where it should*/
             if(oSymTable->first == currentNode) {
                 oSymTable->first = nextNode; 
                 free(currentNode->Key);
                 free(currentNode); 
                 return returned; 
                 }
             /* normal case for in middle so we just make 
             the next node of our previous one just point 
             to the next*/
             previousNode->next = nextNode; 
             free(currentNode->Key);
             free(currentNode); 
             return returned; 
             }
             /* adjust our previous node for the iteration */
         previousNode = currentNode; 
         }
    return NULL; 
}

void SymTable_map(SymTable_T oSymTable,
void (*pfApply)(const char *pcKey, void *pvValue, void *pvExtra),
const void *pvExtra)
{

    /* nodes for traversal */
     struct Node *currentNode; 
    struct Node *nextNode; 

     assert(oSymTable != NULL); 
     assert(pfApply != NULL); 

     for (currentNode = oSymTable->first; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* just call the pfapply function with the parameters and 
        be sure to cast for safety */
        (*pfApply)(currentNode->Key, (void*) currentNode->Value,
         (void*) pvExtra);
    }
}

