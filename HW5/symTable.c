/**
	Homework 5
	SymTable
	Author: Douglas Skrypa
	Date: 2014.04.23
*/

#include <stdio.h>					//IO functions
#include <stdlib.h>					//Memory functions
#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing
#include "symTable.h"

#define DEFAULT_TABLE_SIZE 61
#define HASH_MULTIPLIER 65599

typedef struct SymNode{
	char* key;
	void* value;
	struct SymNode* next;
} Node;

struct SymTable{
	Node** ht;
	int size;
	int htsize;
};

typedef char* string;					//A typical string (character array)

/**
	Hashes a string for use as an index in an array
	@param key the string to be hashed
	@param htsize the size of the hash table
	@return unsigned int hashed key (between 0 and htsize)
*/
static unsigned int hash(const char* key, const int htsize) {
	int i;
	unsigned int h = 0U;
	for (i = 0; key[i] != '\0'; i++){
		h = h * HASH_MULTIPLIER + (unsigned char) key[i];
	}
	return h % htsize;
}

/**
	Free the memory that was previously allocated for the given Node
	@param n a Node to have its memory freed
*/
void freeNode(Node* n){
	if(n != NULL){				//If the current Node is not null
		free(n->key);			//Free memory allocated for its key's string
		free(n->value);
		free(n);				//Free memory allocated for itself
	}
}

/**
	Recursively free the memory that was previously allocated for the given
	Node and the Nodes that it links to
	@param n a Node to have its memory freed
*/
void freeNodes(Node* n){
	if(n != NULL){				//If the current Node is not null	
		freeNodes(n->next);		//Call freeNodes() for the Node it links to
		freeNode(n);			//Then free memory for that Node itself
	}
}

/**
	Initializes a new SymTable, allocating memory for it.
	@return SymTable_T an array of pointers to SymNodes
*/
SymTable_T symTable_create(void){
	SymTable_T st;
	st = (struct SymTable*) malloc(sizeof(struct SymTable));
	st->size = 0;
	st->htsize = DEFAULT_TABLE_SIZE;
	
	Node** ht;					//Allocate memory for htsize pointers to Nodes
	ht = (Node**) malloc(st->htsize * sizeof(Node*));

	if(ht == NULL){				//If unsuccessful, terminate
		fprintf(stderr, "Fatal error: unable to allocate memory for hash table\n");
		exit(-1);
	}
	
	st->ht = ht;
	return st;	//Otherwise, return the pointer for the allocated memory block
}

/**
	Delete the given SymTable, and free the memory that was allocated for it
	@param symTable a pointer to a SymTable
*/
void symTable_destroy(SymTable_T symTable){
	Node** ht = symTable->ht;
	int i;
	for(i = 0; i < symTable->htsize; i++){	//Iterate through each index
		freeNodes(*(ht+i));			//Call recursive method to free Node memory
	}
	free(ht);						//Free memory allocated for the table itself
	free(symTable);
}

/**
	Obtain the number of key, value pairs that are in the given SymTable
	@param symTable a pointer to a SymTable
	@return the number of key, value pairs
*/
int symTable_size(SymTable_T symTable){
	return symTable->size;
}
   
/**
	Inserts a key into the given SymTable
	@param symTable a pointer to a SymTable
	@param key an element to be inserted
	@param value the object associated with the given key
	@param valuesize the size of the given item in bytes
	@return 1 if key was inserted, 0 if it already existed or low memory
*/
int symTable_insert(SymTable_T symTable, const char* key, const void* value, size_t valuesize){
	unsigned int hkey = hash(key, symTable->htsize);
	Node** ht = symTable->ht;
	Node* n;
	
	if(symTable_search(symTable, key, (void*)value, valuesize) == 1){
		return 0;							//If key exists in table, abort
	} else {								//Otherwise,
		n = (Node*) malloc(sizeof(Node));	//Allocate memory for a new Node
		n->key = strdup(key);				//Initialize its values
		void* val;
		val = malloc(valuesize);
		memcpy(val, value, valuesize);
		n->value = val;
		n->next = NULL;		//Leaving this out can have unintended consequences
		
		if(*(ht+hkey) == NULL){		//If no existing Node @ hashed index
			*(ht+hkey) = n;			//Set it to the newly created Node
		} else {					//Otherwise, find the 1st Node in the linked
			Node* m;				//list without a pointer to another Node
			for(m = *(ht+hkey); m->next != NULL; m = m->next){}
			m->next = n;			//Make that Node point to the new Node as
		}							//the next one in the list
		
		symTable->size += 1;		//Increment the # of elements in table
		return 1;					//Return 1 for success
	}
	return 0;						//Return 0 for failure
}

/**
	Searches a SymTable for a given key
	@param symTable a pointer to a SymTable
	@param key an element to search for
	@param value a pointer that will be set to the location of the stored object
	@param valuesize the size of the given item in bytes
	@return 1 if the key was found, 0 otherwise
*/
int symTable_search(SymTable_T symTable, const char* key, void* value, size_t valuesize){
	unsigned int hkey = hash(key, symTable->htsize);
	Node* n;
	Node** ht = symTable->ht;
	//Iterate through Nodes at the index that key hashes to
	for(n = *(ht+hkey); n != NULL; n = n->next){
		if(strcmp(n->key, key) == 0){				//If the keys match
			memcpy(value, n->value, valuesize);		//Fill value with real value
			return 1;								//Return 1 for success
		}
	}
	return 0;										//Return 0 for not in table
}

/**
	Deletes a given key from the given SymTable
	@param symTable a pointer to a SymTable
	@param key an element to search for
	@return 1 if the key was found and its SymNode deleted, otherwise 0
*/
int symTable_delete(SymTable_T symTable, const char *key){
	unsigned int hkey = hash(key, symTable->htsize);
	Node** ht = symTable->ht;
	Node *nFocus, *nPrev;
	for(nFocus = *(ht+hkey); 				//Iterate through Nodes at the
		nFocus != NULL; 					//index that key hashes to
		nPrev = nFocus, nFocus = nFocus->next)
	{
		if(strcmp(nFocus->key, key) == 0){	//If the keys match
			if(nFocus == *(ht+hkey)){		//And its the 1st Node in the list
				*(ht+hkey) = nFocus->next;	//Set it to its next Node (NULL ok)
			} else {						//Otherwise, assign the next Node
				nPrev->next = nFocus->next;	//to previous Node's next pointer
			}								//Then, free the memory that was
			freeNode(nFocus);				//allocated for the deleted Node

			symTable->size -= 1;			//Decrease # of elements in table
			return 1;						//Deletion was successful; return 1
		}
	}
	return 0;								//Unable to find/delete key's Node
}
