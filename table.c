/*
Name: Tousif Chowdhury 
Date: 10/15/2015

*/

#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <stdbool.h>
#include "table.h"
#include "hash.h"
#ifndef UNINSTRUMENTED
#include "amigomem.h"
#endif



/// Create a new hash table.
/// @param hash The key's hash function
/// @param equals The key's equal function for comparison
/// @param print A print function for the key/value, used for dump debugging
/// @exception Assert fails if can't allocate space
/// @return A newly created table
Table* create(long (*hash)(void* key), bool (*equals)(void* key1, void* key2), void (*print)(void* key1, void* key2)){
    //Malloc space for the table 
    //And fill in the intial heap accordingly 
    Table* table1 = (Table*)malloc(sizeof(Table));
    //ADD ASSERT IF NULL
    if(table1 == NULL){
	fprintf(stderr,"table::create() failed to allocate space");
	assert(NULL);
    }
    //fill out the hash table 
    table1 -> hash = hash;
    table1 -> equals = equals;
    table1 -> print = print; 
    table1 -> size = 0; 
    table1 -> capacity = INITIAL_CAPACITY;
    table1 -> collisions = 0; 
    table1 -> rehashes = 0;

    table1 -> table = malloc(sizeof(Entry)*INITIAL_CAPACITY);
    if(table1->table == NULL){
	fprintf(stderr,"NULL");
	assert(NULL);
    }
    for(size_t i = 0; i < table1 -> capacity; i++){
	table1 -> table[i] = (Entry*)calloc(1,sizeof(Entry));
    }
    

    return table1;


}

/// Destiroy a table
/// @param t The table to destroy
void destroy(Table* t){
    //Frees every space malloced
    for( size_t i =0; i < t->capacity; i++){

	free(t ->table[i]);
    }

    free(t -> table);
    free(t);

}

/// Print out information about the hash table (size,
/// capacity, collisions, rehashes).  If full is
/// true, it will also print out the entire contents of the hash table,
/// using the registered print function with each non-null entry.
/// @param t The table to display
/// @param full Do a full dump of entire table contents
void dump(Table* t, bool full){
    //Pretty much a print method to display information
    printf("Size: %zu\nCapacity: %zu\nCollisions: %zu\nRehashes: %zu\n", t -> size, t -> capacity, t -> collisions, t -> rehashes);
    if(full){
	for(size_t i = 0; i < t -> capacity; i++){
	    printf("%zu : ", i);
	    if(t->table[i]->key == NULL){
		printf("null\n");
	    } else {
		printf("(");
		t -> print(t -> table[i] -> key, t -> table[i] -> value);
		printf(")\n");
	    }
	}
    }

}



/// Get the value associated with a key from the table.  This function
/// uses the registered hash function to locate the key, and the
/// registered equals function to check for equality.
/// @pre The table must have the key, or else it will assert fail
/// @param t The table
/// @param key The key
/// @return The associated value of the key
void* get(Table* t, void* key){
    //Searchs for a key a returns a value 
    long index = t -> hash(key)% t -> capacity; 
    long startIndex = index; 
    while(t -> table[index]->key != NULL && !(t->equals(t->table[index]->key,key)) ){
	index = (index + 1) % t -> capacity; 
         t->collisions+=1;
	if (index == startIndex){
	    fprintf(stderr, "Hash table does not contain key.");
	    assert(NULL);
	}


    }
    if (t->table[index]->key == NULL){
	fprintf(stderr, "Hash table does not contain key.");
	assert(NULL);
    }
    else{
    
	return t->table[index]-> value;
    }


}
 


/// Check if the table has a key.  This function uses the registered hash
/// function to locate the key, and the registered equals function to
/// check for equality.
/// @param t The table
/// @param key The key
/// @return Whether the key exists in the table.
bool has(Table* t, void* key){
    //Loops through the tables and sees 
    //if the the table has a key or not 
    long index = t->hash(key) % t->capacity;
    long startIndex = index;
    while (t->table[index]->key != NULL && !t->equals(t->table[index]->key,key)){
	index = (index + 1)% t->capacity; 
	t->collisions++;
	if (index == startIndex){
	    return false;
	    }
	}
    return t->table[index]->key != NULL && (t->equals(t->table[index]->key,key)); 
}

/// Get the collection of keys from the table.  This function allocates
/// space to store the keys, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of keys
void** keys(Table* t){
    //Malloc space for an array
    //add alll the keys into the array
    size_t counter =0; 
    void **arr = malloc(sizeof(void *) * t->capacity);
    for(size_t i = 0; i< t-> capacity; i++){
	if(t->table[i]->key != NULL){
	    arr[counter] = t -> table[i] -> key;
	    counter++; 
	}
    }
    return arr; 

}

/// Add a key value pair to the table, or update an existing key's value.
/// This function uses the registered hash function to locate the key,
/// and the registered equals function to check for equality.
/// @param t The table
/// @param key The key
/// @param value The value
/// @exception Assert fails if can't allocate space
/// @return The old value in the table, if one exists.
void* put(Table* t, void* key, void* value){
    //Puts a key, value into the table. 
    //If it goes past load threshhold, then 
    //the table will rehash 
    if(((double)t->size/(double)t->capacity) >= (double)LOAD_THRESHOLD) {
	Table *t2 = (Table*)malloc(sizeof(Table));
	if(t2 == NULL){
	    fprintf(stderr,"table::create() failed to allocate space");
	    assert(NULL);

	}
	t2 -> hash = t->hash;
	t2 -> equals = t->equals;
	t2 -> print = t->print; 
	t2 -> size = 0; 
	t2 -> capacity = t->capacity*RESIZE_FACTOR;
	t2 -> collisions = t->collisions; 
	t2 -> rehashes = t->rehashes;
 
	t2 -> table = malloc(sizeof(Entry)*t2->capacity);
	 //if assert
	if(t2->table == NULL){
	    fprintf(stderr,"table::create() failed to allocate space");
	    assert(NULL);

	}
	for(size_t i = 0; i < t2 -> capacity; i++){
	    t2 -> table[i] = (Entry*)calloc(1,sizeof(Entry));
	}
	for(size_t i =0; i < t->capacity; i++){
	    if(t->table[i]->key != NULL){
		put(t2,t->table[i]->key,t->table[i]->value);
	    }
	    free(t->table[i]);
	}
	free(t->table);
	t->table = t2->table; 
	t->capacity = t2->capacity;
	t->rehashes = t2->rehashes + 1;
	t->collisions = t2->collisions;
	free(t2);

    }


    //Does the normal hashing
    void* oldValue = NULL;
    long index = t->hash(key) % t->capacity;

    while (t->table[index]->key != NULL && !t->equals(t->table[index]->key,key)){
        index = (index + 1)% t->capacity; 
        t->collisions++;

    }
    if (t->table[index]-> key == NULL){
        t->table[index]->key = key; 
        t->table[index]->value = value; 
        t->size = t->size +1; 
    }
    else if (t->equals(t->table[index]-> key, key)){
        oldValue = t->table[index]->value;
        t->table[index]->value = value;
    }

    return oldValue;


}

/// Get the collection of values from the table.  This function allocates
/// space to store the values, which the caller is responsible for freeing.
/// @param t The table
/// @exception Assert fails if can't allocate space
/// @return A dynamic array of values
void** values(Table* t){
    //Malloc space for an array 
    //Add all the values into there 
    size_t counter=0; 
    void **arr = malloc(sizeof(void *)* t->capacity);
    for(size_t i=0; i < t-> capacity; i++){
	if(t->table[i]->key != NULL){
	    arr[counter] = t-> table[i] -> value;
	    counter++; 
	}
    }
    return arr; 

}


