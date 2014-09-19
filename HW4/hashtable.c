/**
	Homework 4
	Hashtable
	Author: Douglas Skrypa
	Date: 2014.04.10

	*****	Test with Valgrind	*****
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>					//String functions
/*
char *stpcpy (char *dest,const char *src) -- Copy one string into another.
int strcmp(char *string1,const char *string2) - Compare string1 and string2 to determine alphabetic order.
char *strcpy(char *string1,const char *string2) -- Copy string2 to stringl.
char *strerror(int errnum) -- Get error message corresponding to specified error number.
size_t strlen(const char *string) -- Determine the length of a string.
char *strncat(char *string1, char *string2, size_t n) -- Append n characters from string2 to stringl.
int strncmp(char *string1, char *string2, size_t n) -- Compare first n characters of two strings.
char *strncpy(char *string1,const char *string2, size_t n) -- Copy first n characters of string2 to stringl .
int strcasecmp(const char *s1, const char *s2) -- case insensitive version of strcmp().
int strncasecmp(const char *s1, const char *s2, int n) -- case insensitive version of strncmp(). 
*/
#include <ctype.h>					//Character conversions and testing

//Structs & typedefs
typedef char *string;				//Defines a type 'string' for ease of use
struct NodeType {
	string key;
	int count;
	struct NodeType *next;
};
typedef struct NodeType Node;

//Constant definitions
#define HASH_MULTIPLIER 65599

//Function prototypes
unsigned int hash(const string);
Node **ht_create();
int ht_insert(Node**, const string);
int ht_delete(Node**, const string);
int ht_search(Node**, const string, int*);
void ht_print(Node**);
void ht_destroy(Node**);
int isNum(const string);
/*	String functions	*/
string truncate(const string, int);
int getLength(const string);

//Global variables
int htsize = 0;

/*
1. insert <key>:	insert <key> into the hash table. If the key cannot be inserted (i.e., a node cannot be created), issue the message “Insert failed”
2. delete <key>:	delete <key> from the hash table. If the key is not in the table, issue the message “Key is not in table”
3. search <key>: 	search for <key> in the hash table. If found, print the key and its current count; otherwise issue the message “Key not found”
4. print: 			print the current contents of the hash table
5. quit: 			destroy the hash table and terminate the program
*/

int main(int argc, string* argv){
	if(argc < 2 || !isNum(argv[1])){
		printf("Invalid input\n");
		return -1;
	}
	htsize = atoi(argv[1]);
	printf("Hash Table HT[0:%s]\n", htsize);

	int canRun = 1;
	string rawInput = NULL;
	size_t inputLen = 0;

	while((canRun == 1)){
		printf(" » ");
		//Get user input and remove trailing spaces
		getline(&rawInput, &inputLen, stdin);
		string input = truncate(rawInput, getLength(rawInput));

		//Tokenize user input for processing a command + args
		string token = strtok(input, " ");
		string command = strdup(token);
		token = strtok(NULL, " ");
		string args = NULL;

		while(token != NULL){			//Let args include spaces
			if(args == NULL){
				args = strdup(token);
			} else {
				args = strcat(args, " ");
				args = strcat(args, token);
			}
			token = strtok(NULL, " ");
		}

		//Choose a function based on user input
		if((strcasecmp(command, "quit") == 0) || (strcasecmp(command, "exit") == 0)){
			canRun = 0;
		} else if(strcasecmp(command, "search") == 0){
		} else if(strcasecmp(command, "insert") == 0){
		} else if(strcasecmp(command, "delete") == 0){
		} else{
			printf("Invalid command: %s\n", input);
		}
	}
	free(rawInput);
	return 0;
}

/**
	Hashes a string
	@param key the string to be hashed
	@return unsigned int hashed key (between 0 and htsize)
*/
unsigned int hash(const string key){
	int i;
	unsigned int h = 0U;
	for (i = 0; key[i] != '\0'; i++){
		h = h * HASH_MULTIPLIER + (unsigned char) key[i];
	}
	return h % htsize;
}

/**
	Creates a new hash table.
	@return Node** an array of pointers to Nodes
*/
Node** ht_create(){
	Node** ht;
	ht = (Node**) malloc(htsize * sizeof(Node*));

	if(ht == NULL){
		fprintf(stderr, "Fatal error: unable to allocate memory for hash table\n");
		exit(-1);
	}
	return ht;
}

/**
	Inserts a key into the given hash table
	@param ht a pointer to a hash table
	@param key an element to be inserted
	@return int 1 if the operation was successful, 0 otherwise
*/
int ht_insert(Node **ht, const string key){
	int c = 0;
	unsigned int hkey = hash(key);
	if(ht_search(ht, key, &c) == 1){
		ht[hkey]->count++;
	} else {
		ht[hkey] = (Node*) malloc(sizeof(Node));
		ht[hkey]->key = strdup(key);
		ht[hkey]->count = 1;
	}
	//insert key into the hashtable ht. If key is not in the table at the time of insertion, create a node for key and
	//initialize its count to 1. If key is already in the table, increment its count by 1. Return
	//1 on success, else return 0 (i.e., if key is not in the table and a new node cannot be created).
	return 0;
}

/**
	Deletes a given key from the given hash table
	@param ht a pointer to a hash table
	@param key an element to be removed
	@return int 1 if key was in table & operation was successful, 0 otherwise
*/
int ht_delete(Node **ht, const string key){
	int c = 0;
	unsigned int hkey = hash(key);
	if(ht_search(ht, key, &c) == 1){
		Node *n = ht[hkey];
		while(*n != NULL){
			if(strcmp((*n).key, key) == 1){
				free((*n).key);
				*n = (*n).next;
				return 1;
			}
			n = (*n).next;
		}
	}
	return 0;
}

/**
	Searches for occurrences of the given key in the given hash table
	@param ht a pointer to a hash table
	@param key an element to search for
	@param count a pointer to an int that will be filled by the number of the given element found
	@return int 1 if the given key was found, 0 otherwise
*/
int ht_search(Node **ht, const string key, int *count){
	unsigned int hkey = hash(key);
	Node *n = ht[hkey];						//Free n?
	while(*n != NULL){
		if(strcmp((*n).key, key) == 1){
			*count = (*n).count;
			return 1;
		}
		n = (*n).next;
	}
	return 0;
}

/**
	Prints the contents of the given hash table
	@param ht a pointer to a hash table
*/
void ht_print(Node **ht){
	//print all keys current stored in the hash table ht.  Specifically, iterate over the buckets of the hash table and print the <key, count> pairs hashed to each bucket.
}

/**
	Delete the given hash table
	@param ht a pointer to a hash table
*/
void ht_destroy(Node **ht){
	//destroy the hash table ht by freeing all the space allocated to the table.
}

/*
	===========================================================================
		Non-hash table functions
	===========================================================================
*/

/**
	Determines if a given string is a positive integer
	@param s a string to test
	@return 1 if all characters in the given string are integers, 0 otherwise
*/
int isNum(const string s){
	int i;
	for(i = 0; i < strlen(s); i++){
		if(!isdigit(s[i])){
			return 0;
		}
	}
	return 1;
}

/**
	Find the number of characters in a given string.  Main intended use is for
	finding where to truncate raw input from getline().
	@param s a string to be measured
	@return int number of characters in s
*/
int getLength(const string s){
	int c = 0;
	while((s[c] != '\n') && (s[c] != EOF) && (s[c] != '\0')){
		c++;	//Count chars until new line, end of file, or null char found
	}
	return c;
}

/**
	Truncate the given string of raw input, as is obtained via getLine().
	@param raw a string of raw input
	@param length number of characters in raw
	@return string containing everything from the beginning of raw until '\n'
*/
string truncate(const string raw, int length){
	char newString[length+1];
	int i;
	for(i = 0; i < length; i++){
		newString[i] = raw[i];		//Reconstruct char array to specified point
	}
	newString[length] = '\0';
	return strdup(newString);
}
