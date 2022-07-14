# SymTable

Here, we have an ADT called SymTable. It is a symbol table that contains key-value pairs. The key must be a string, but the value can be anything. This table provides a wide range of funtionality, defined in the interface, but some methods include retrieving, putting, or removing bindings. Included is an interface, and two implementations: a linked list implementation, and a resizing array implementation. Please pay attention to the comments if using the resizing array implementation because it involves dynamically allocated memory. 

# To use
1. Download "symtablelist.c", "symtablehash.c", and "symtable.h"
2. These are implementations that require a client, YOU, so you may use these implementations however you want
    - Be sure you "include" the .h files in your c file with "#include symtable.h" at the top of your file
    - if your file is called "client.c" then you may create your executable with the following command (using the hash table implementation): 
        - gcc client.c symtablehash.c -o client
