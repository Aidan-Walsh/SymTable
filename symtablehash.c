/*------------------------------------------------------------------*/
/* symtablehash.c                                                   */
/* Author: Aidan Walsh                                              */
/*------------------------------------------------------------------*/

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtable.h"

/* array that stores the sizes of the changing hash table */
static size_t binCountArray[8]={509, 1021, 2039, 4093, 8191, 16381, 
32749, 65521};

/* structure that describes SymTable_T */
struct SymTable 
{   
    /* this index is the index into the global variable binCountArray
    that gets the correct size of the hash table */
    int index; 
   

    /* need to keep track of count of all bindings we add */ 
    size_t count; 

    /* our array of Node pointers that is our hash table */
    struct Node **table;

     

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
} ;

/* Return a hash code for pcKey that is between 0 and uBucketCount-1,
        inclusive. */
        
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


static void SymTable_expand(SymTable_T oSymTable) {
size_t i; 

/*this stores the new hash table*/
struct Node **newTable;

/*nodes for traversal*/
struct Node *currentNode; 
struct Node *nextNode; 

/*node that stores the current node we are looking at (likely no need for it)*/
struct Node *testNode; 

/*value that stores hash index*/
size_t newIndex; 
assert(oSymTable != NULL);

/*increment the index for the new size of hash table*/
 (oSymTable->index)++;  

     newTable = calloc(binCountArray[oSymTable->index], 
     sizeof(struct Node*)); 
     
/*go through entire hash table in use*/
for (i = 0; i< binCountArray[(oSymTable->index)-1]; i++) {
         
         /*going through each index*/
         for (currentNode = oSymTable->table[i]; (currentNode != NULL); 
    currentNode = nextNode)
    { 
        
       /* get hashing index and make pointer of new hash table point to
        the current node we are looking at*/
        nextNode = currentNode->next; 
        testNode = currentNode; 
        newIndex = SymTable_hash(currentNode->Key, 
        binCountArray[oSymTable->index]); 

        /* if we are moving it to the front, make sure its next node NULL
        else, make it just the first node */
       if (newTable[newIndex] == NULL) {
       testNode->next = NULL; 
       newTable[newIndex] = testNode;
       }
       else {
           testNode->next = newTable[newIndex]; 
           newTable[newIndex] = testNode; 
       }

} 

} 
 
/* point current table to new table*/
oSymTable->table = newTable; 

}


SymTable_T SymTable_new(void)
{
    SymTable_T oSymTable; 

    /* allocate memory for size of SymTable struct */
    oSymTable = (SymTable_T)malloc(sizeof(struct SymTable)); 
    if (oSymTable == NULL) { 
        return NULL; 
    }

    /* adjust index and count values then allocate memory for each 
    of the first nodes of the hash table and ensure there is enough
    memory */
     oSymTable->index = 0; 
     oSymTable->count = 0;  
     oSymTable->table = calloc(509, sizeof(struct Node*)); 

    return oSymTable; 

}

void SymTable_free(SymTable_T oSymTable) {

    /* create nodes for traversal */
    struct Node *currentNode; 
    struct Node *nextNode; 
    size_t i; 
    
   assert(oSymTable != NULL); 

    /* go through entire hash table */
    for (i = 0; i < binCountArray[oSymTable->index]; i++) {
        /* while a node is not null, we need to free the key, then the
        node itself */
            for (currentNode = oSymTable->table[i]; 
            currentNode != NULL; currentNode = nextNode) {
                nextNode = currentNode->next; 
                free((void*)currentNode->Key);
                free(currentNode); 
                }       
        
    }
    free(oSymTable); 
}

size_t SymTable_getLength(SymTable_T oSymTable) {
    assert(oSymTable!=NULL);
    return oSymTable -> count; 
}

int SymTable_put(SymTable_T oSymTable, const char *pcKey, 
const void *pvValue) {

    /* new nodes for traversal and store our new node into newNode
    and store hashing index */
    struct Node *currentNode; 
    struct Node *nextNode; 
    struct Node *newNode; 
    size_t newIndex;  

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    newIndex = SymTable_hash(pcKey, 
    binCountArray[oSymTable->index]); 
    
    for (currentNode = oSymTable->table[newIndex]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* ensure that it is not present by comparing keys */
        if (strcmp(pcKey, currentNode->Key) == 0) {
            return 0; 
        }
    }
     
     /* since not present, we increment count, allocate memory for the
     Node, then the key and adjust values */
     (oSymTable->count)++; 

    /* since we might be hitting a boundary, we test to see this, 
    make sure we dont go over the hash table limit (given by 
    the 7th index of the array), and call expand
    if we need to and get the new hash index since we have just 
    changed the size */
     if (oSymTable->count == binCountArray[oSymTable->index] &&
      oSymTable->index < 7) {
         
            SymTable_expand(oSymTable); 
            newIndex = SymTable_hash(pcKey, binCountArray
            [(oSymTable->index)]); 
      } 
        
    newNode = (struct Node*)malloc(sizeof(struct Node)); 
            if (newNode == NULL) {
                return 0; 
            }
            
            newNode->Value = pvValue; 
           
          
            newNode->next = oSymTable->table[newIndex]; 
            
            oSymTable->table[newIndex] = newNode;
            
            /* allocate that extra 1 memory for the terminating 
            character */
            newNode->Key = (char*)malloc((strlen(pcKey)+1)*
            sizeof(char));
             
            if(newNode->Key == NULL) {
                return 0; 
            }
            newNode->Key = strcpy(newNode->Key, pcKey);
           
            return 1; 
}

void *SymTable_replace(SymTable_T oSymTable, const char *pcKey,
const void *pvValue) {

    /* nodes for traversal, store value to be returned and new
    hashing index */
    struct Node *currentNode; 
    struct Node *nextNode; 
     void *returned; 
    size_t newIndex; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    newIndex = SymTable_hash(pcKey, 
    binCountArray[oSymTable->index]); 
    for (currentNode = oSymTable->table[newIndex]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* if we find a match to key, we store value, adjust value
        then return */
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             returned = (void*) (currentNode->Value); 
            currentNode->Value = pvValue; 
            return returned; 
        }
    }
       
    return NULL; 
}

int SymTable_contains(SymTable_T oSymTable, const char *pcKey) {

    /* nodes for traversal and store hash table index */
     struct Node *currentNode; 
    struct Node *nextNode; 
     size_t newIndex;

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    newIndex = SymTable_hash(pcKey, 
     binCountArray[oSymTable->index]);
    for (currentNode = oSymTable->table[newIndex]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 
        /* if we find a match to the Key, we just return 1 */
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             return 1; 
         }
    }
    return 0; 

}

void *SymTable_get(SymTable_T oSymTable, const char *pcKey) {

    /* nodes for traversal and newIndex that stores hash table index*/
    struct Node *currentNode; 
    struct Node *nextNode; 
     size_t newIndex; 

     assert(oSymTable != NULL); 
    assert(pcKey != NULL); 

    newIndex = SymTable_hash(pcKey, binCountArray[oSymTable->index]);
    for (currentNode = oSymTable->table[newIndex]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* if we find a match to the key, we just return the value */
        if (strcmp(currentNode->Key, pcKey) == 0)
         {
             return (void*) currentNode->Value; 
         }
    }
    return NULL; 
}

void *SymTable_remove(SymTable_T oSymTable, const char *pcKey) {

    /* nodes for traversal, previousNode to store the previous node, 
    store the value to be returned, and the hash table index */
    struct Node *currentNode; 
    struct Node *nextNode; 
    struct Node *previousNode; 
    void *returned; 
    size_t newIndex; 

    assert(oSymTable != NULL); 
    assert(pcKey != NULL); 
   

    newIndex = SymTable_hash(pcKey, binCountArray[oSymTable->index]);
    for (currentNode = oSymTable->table[newIndex]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

         /* if we find the Key, then we remove. We look at the index
         provided by the hashing function */
        if (strcmp(currentNode->Key, pcKey) == 0) {
             (oSymTable->count)--;
             returned = (void*) currentNode->Value; 
             /* corner case for only 1 node present, so we have to set 
             the first Node to NULL*/
             if(oSymTable->table[newIndex] == currentNode && nextNode == NULL) {
                 oSymTable->table[newIndex] = nextNode; 
                 free(currentNode->Key);
                 free(currentNode); 
                 oSymTable->table[newIndex] = NULL; 
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
             if(oSymTable->table[newIndex] == currentNode) {
                 oSymTable->table[newIndex] = nextNode; 
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
const void *pvExtra) {

    /* nodes for traversal */
    struct Node *currentNode; 
    struct Node *nextNode; 
    size_t i; 

     assert(oSymTable != NULL); 
     assert(pfApply != NULL); 

     /* go through entire size of hash table */
    for (i = 0; i < binCountArray[oSymTable->index]; i++) {

        /* look for non NULL nodes and map */
     for (currentNode = oSymTable->table[i]; currentNode != NULL; 
    currentNode = nextNode)
    {
        nextNode = currentNode->next; 

        /* call pfapply to arguments */
        (*pfApply)(currentNode->Key, (void*) currentNode->Value,
         (void*) pvExtra);
    }
    }
}
