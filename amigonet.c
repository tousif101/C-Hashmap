/*
Author: Tousif Chowdhury
Date: 10/23/15
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> 
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "amigonet.h"
#include "table.h"
#include "hash.h"


#ifndef UNINSTRUMENTED
#include "amigomem.h"
#endif


static Table* friendstab;


typedef struct User_S{
    
    char name[MAX_NAME_LENGTH+1]; 
     //char* name;
     //char* birthday;
    char birthday[10];
    struct User_S *friends; 
}User_S;

void strStringPrint(void* key, void* value) {
    printf("%s : %s", (char*)key, (char*)value);
}
/**
 * Initialize the system.
 * (This is where some memory is initially allocated.)
 */
void create_amigonet(){
    //make the table where you keep your friends
    //Add the birthday with the name run it through hash code
    //Modulo by the size of the table, to get the index 
    friendstab = create(strHash, strEquals, strStringPrint);
    
}


/**
 * Shut down the system.
 * (This is where a bunch of memory is freed.)
 */

 
void destroy_amigonet(){
    //worry about how you made the friends 
    //destroy(friends);
    //go through ever users friend list 
    //and the friends in the linked list 
    //and free everyone 
    //free the keys as well 
   // void **arr = keys(friendstab);
    for(size_t i =0; i<friendstab->capacity;i++){
	 //User_S* temp_node = get(friendstab,arr[i]);
	 if( friendstab->table[i]!= NULL){
	     //temp_node =temp_node->friends;
	     free(friendstab->table[i]->key);
	     free(friendstab->table[i]->value);
	 }
	 //free(temp_node);
	 //free(arr[i]);
    }
    destroy(friendstab);

}


/**
 * Add a new user with initially no friends.
 * The parameters 'name' and 'birthday' are used to initialize the new user entry.
 * Note: they must be copied to prevent the caller from changing
i * the them later.
 * If the name already exists, then this function does nothing, except for
i * print an error message.
 */
void addUser( const char *name, const char *birthdate ){
    //Add the friends given as an entry in hastable
    //create a user_S struct with the given info 
    //make the key; name and birthday string cat use it to make key 
    //call put takes in value and key 
    //char *name;
    //char *birthday;
    char* key = malloc(sizeof(name)+sizeof(birthdate)+1);

    strncpy(key,name,strlen(name)+2);

    // char* extra = malloc()

    // strncpy(extra,name);
    strncat(key,birthdate,strlen(birthdate)+2);


    User_S* f1 = (User_S*)malloc(sizeof(User_S));

    //f1->name = 
    strcpy(f1->name,name);
    //f1->name= strcpy(name);
    //f1->birthday = 
    strcpy(f1->birthday,birthdate);

    f1->friends = NULL; 
    if(has(friendstab,key)){
	printf("User already exists");

    }

    put(friendstab,key,f1);

}


/**
 * Print out the specified user's friends in the order that they were
 * "friended"
 */
void printAmigos( struct User_S *user ){
    //Call values of the users? 
    
    //loop through user 
    //in the loop print out each user friends 
    //call print in the hash table 
    User_S* curr = user->friends; 
    while(curr != NULL){
	curr=user->friends; 
	
	friendstab->print(curr->name,curr->birthday);
	printf("Friends: %s", curr->name);
    }
}


/**
 * Print the number of users.
 */
void printNumUsers(void){
    //print the size of the table?
    printf("Number Users: %zu \n",friendstab->size);

}

/**
 * Locate a user structure using the user's name and birthdate as a key.
 * User structures are needed for the addAmigo, removeAmigo,
 * and printFriends functions.
 * If the user does not exist, NULL is returned.
 */
struct User_S *findUser( const char *name, const char *birthdate ){
    char* key = malloc(strlen(name)+strlen(birthdate)+1);

    strncpy(key,name,strlen(name));

    strncat(key,birthdate,strlen(birthdate));

    if(has(friendstab,key)){
	return (User_S*) get(friendstab, key);

    }
    else{
	return (NULL);
    }
    free(key);

}


/**
 * Add a friend (the "amigo") to the user. This should be a two-way
 * addition. If the two users are already friends, this function
i * does nothing except print an error message. 
 */
void addAmigo( struct User_S *user, struct User_S *amigo ){
    //get the user key and value in the hash table 
    //loop through there friends 
    //add the amigo to the user as a linked list in the end  
    //For loop through user->friends 
    /*
    if(user->friends == NULL){
	user->friends = amigo;
    }
    */
    //Do all the checks 
    /*
    if(user->name == amigo-> name && user->birthday == amigo->birthday){
	printf("Cant request yourself \n");
	return; 
    }
    */
    //if(!has(friendstab,user->name )
    User_S* curr = user->friends;
    while(curr != NULL){
	curr = curr-> friends; 

	//check if the current user is equal to the amigo you have 
	// it means you already have that friend.

	if(friendstab->equals(curr->name,amigo->name)&&
	    friendstab->equals(curr->birthday,amigo->birthday)){
	    printf("User in the table \n");
	    return; 
	
    }	
    curr->friends = (User_S*)malloc(sizeof(User_S));

    strcpy(curr->friends->name,amigo->name);
    strcpy(curr->friends->birthday,amigo->birthday);
    curr->friends = NULL;
   }
}



/**
 * "Un-friend" two users. This is, again, a two-way operation.
 * If the two users were not friends, this function does nothing,
 * except print an error message.
 */
void removeAmigo( struct User_S *user, struct User_S *ex_amigo ){
    //do the checks and print error 
    //what happens if there is only one element 
    //if the friend is the first the one then do something 
    //when its not the first

    //need temp node to store ex friend 
    //free temp node 

    User_S* curr = user->friends;
    while(curr != NULL && friendstab->equals(user->name,ex_amigo->name)){
	
	curr=curr->friends; 

    }
    User_S* temp_node;
    temp_node = user->friends;
    user->friends = temp_node->friends;
    free(temp_node);

}


