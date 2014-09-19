/**
	Homework 3
	String table
	Author: Douglas Skrypa
	Date: 2014.03.15
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXSIZE 20			//Maximum number of entries in the table

typedef char *string;		//define a type 'string' for ease of use

/*	Table operation functions	*/
void insert(const string, string*, int*);
void delete(const string, string*, int*);
void sortTable(string*, int*);
int contains(const string, string*, int*);
void printContents(string*, int*);
/*	String functions	*/
string truncate(const string, int);
string toLower(const string);
int getLength(const string);

int main(void)
{
	int canRun = 1;
	printf("String Table\n");
	printf("============\n");
	printf("Operations allowed are:\n");
	printf("1) insert string: insert a string into the table\n");
	printf("2) delete string: delete a string from the table\n");
	printf("3) print: print the entire table in sorted order\n");
	printf("4) quit: quit the program\n");
	printf("The table is initially empty.\n");

	string rawInput = NULL;
	size_t inputLen = 0;

	string table[MAXSIZE];
	int count = 0;

	while((canRun == 1)){
		printf(" » ");
		//Get user input and remove trailing spaces
		getline(&rawInput, &inputLen, stdin);
		string input = truncate(rawInput, getLength(rawInput));

		//Tokenize user input for processing a command + args
		string token = strtok(input, " ");
		string command = toLower(token);
		token = strtok(NULL, " ");
		string args = NULL;

		while(token != NULL){			//Let args include spaces
			token = toLower(token);		//All input converted to lower case
			if(args == NULL){
				args = strdup(token);
			} else {
				args = strcat(args, " ");
				args = strcat(args, token);
			}
			token = strtok(NULL, " ");
		}

		//Choose a function based on user input
		if((strcmp(command, "quit") == 0) || (strcmp(command, "exit") == 0)){
			canRun = 0;
		} else if(strcmp(command, "print") == 0){
			printContents(table, &count);
		} else if(strcmp(command, "insert") == 0){
			insert(args, table, &count);
		} else if(strcmp(command, "delete") == 0){
			delete(args, table, &count);
		} else{
			printf("Invalid command: %s\n", input);
		}
	}
	free(rawInput);
	return 0;
}

/**
	Searches a table for a given string.
	@param s a string to search for in table
	@param *table pointer to an array of strings
	@param *count pointer to the number of elements in table
	@return 1 if s was found in table, 0 otherwise
*/
int contains(const string s, string *table, int *count){
	int i;
	for(i = 0; i < *count; i++){
		if(strcmp(*(table+i), s) == 0){
			return 1;
		}
	}
	return 0;
}

/**
	Inserts the given string into the given table if it is not full, then sorts 	it.  Does not allow duplicate entries.
	@param s a string to insert into table
	@param *table pointer to an array of strings
	@param *count pointer to the number of elements in table
	@see sortTable(string*, int*)
*/
void insert(const string s, string *table, int *count){
	if(s == NULL){
		printf("Error: missing argument.\n");
	} else if(contains(s, table, count)){
		printf("Error: '%s' already exists in the table.\n", s);
	} else if(*count <= MAXSIZE){
		*(table+*count) = strdup(s);	//Place the val of s in the table
		(*count)++;						//Increment count of table's elements
		sortTable(table, count);		//Re-sort because of new element
	} else if(*count > MAXSIZE){
		printf("Error: table is full. Unable to insert element: %s\n", s);
	} else {	//Should never happen; included to distinguish from known causes
		printf("Unknown error occurred while attempting to insert '%s'\n", s);
	}
}

/**
	Remove the given string if it exists in the given table.
	@param s a string to remove from table
	@param *table pointer to an array of strings
	@param *count pointer to the number of elements in table
*/
void delete(const string s, string *table, int *count){
	if(s == NULL){
		printf("Error: missing argument.\n");
	} else if(contains(s, table, count)){
		int i, index = -1;
		for(i = 0; i < *count; i++){
			if(strcmp(*(table+i), s) == 0){
				index = i;
				break;		//Index of desired string found; stop searching
			}
		}
		if(index != -1){	//CYA
			for(i = index; i < (*count-1); i++){
				*(table+i) = *(table+i+1);	//Shift each element back 1 place
			}
			*(table+*count-1) = NULL;	//Delete the value from the last index
			(*count)--;					//Subtract 1 from count of elements
		} else {
			printf("Unknown error occurred while searching for index of element '%s'\n", s);
		}
	} else {
		printf("Error: could not find '%s' in table.\n", s);
	}
}

/**
	Sort the elements of table according to the Insertion Sort algorithm.
	@param *table pointer to an array of strings
	@param *count pointer to the number of elements in table
*/
void sortTable(string *table, int *count){
	int i, j;
	string key;
	for (i = 1; i < *count; i++){
		key = *(table+i);
		j = i - 1;

		while(j >= 0 && (strcmp(key, *(table+j)) < 0)){
			*(table+j+1) = *(table+j);
			j--;
		}
		*(table+j+1) = key;
	}
}

/**
	Print the contents of table to stdout.
	@param *table pointer to an array of strings
	@param *count pointer to the number of elements in table
*/
void printContents(string *table, int *count){
	int i;
	for(i = 0; i < *count; i++){
		printf("%s\n", *(table+i));
	}
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
	Convert a given string to all lower case.
	@param s a string to be converted
	@return string with the content of s in lower case
*/
string toLower(const string s){
	int i = 0;
	char inLower[strlen(s)];			//Create new char array same size as s
	for(i = 0; i < strlen(s); i++){		//Individually lower each char's case
		inLower[i] = tolower(s[i]);
	}
	return strdup(inLower);
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
