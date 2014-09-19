/**
	Misc. useful utilities & definitions
	Author: Douglas Skrypa
	Date: 2014.04.27
*/

#include <stdio.h>					//IO functions
#include <string.h>					//String functions
#include <ctype.h>					//Character conversions and testing
#include "dsUtils.h"

/*	Vector functions	===================================================	*/
/*
	Vector is an incomplete implementation of an expandable array.  It is
	incomplete in that it does not have any functions for removing individual
	items from the array.  That functionality was not necessary for its current
	intended use.
*/

/**
	Initialize memory for a vector
	@return a pointer to the newly created vector
*/
vector* createVector(){
	vector* v;
	v = (vector*) malloc(sizeof(vector));
	v->contents = NULL;
	v->elements = 0;
	v->capacity = 0;
	return v;
}

/**
	Get the element from the given vector at the given index
	@param v a vector
	@param index the index of the desired item
	@return the string at the given index
*/
string getElement(vector* v, int index){
	if((0 <= index) && (index < v->elements)){
		return *(v->contents+index);
	}
	return NULL;
}

/**
	Get the number of elements in the given vector
	@param v a vector
	@return the number of strings stored in the given vector
*/
int getSize(vector* v){
	return v->elements;
}

/**
	Add a string to the given vector
	@param v a vector
	@param s a string
	@return 1 on success, 0 on failure (insufficient memory)
*/
unsigned insertElement(vector* v, string s){
	unsigned res = 1;
	v->elements += 1;
	if(v->elements > v->capacity){
		res = expandVector(v);
	}
	if(res){
		*(v->contents+(v->elements)-1) = strdup(s);
		return 1;
	}
	return 0;
}

/**
	Increase the capacity of the given vector by 1
	@param v a vector
	@return 1 on success, 0 on failure (insufficient memory)
*/
unsigned expandVector(vector* v){
	v->capacity += 1;
	string* elements;
	elements = (string*) realloc(v->contents, (v->capacity) * sizeof(string));
	if(elements){
		v->contents = elements;
		return 1;
	}
	return 0;
}

/**
	Release memory allocated to items in the given vector
	@param v a vector
*/
void freeVector(vector* v){
	if(v->elements > 0){			//Prevent problems when not filled
		int i;
		for(i = 0; i < v->elements; i++){
			free(*(v->contents+i));
		}
	}
	free(v->contents);				//Free memory allocated for strings
	free(v);
}

/*	Other functions	=======================================================	*/

/**
	Retrieve a string of user input from stdin
	@return a string of user input
*/
string getInput(){
	char input[BUFSIZ], *p;			//Use built-in BUFSIZ for buffer size
	if(fgets(input, sizeof(input), stdin) != NULL){	//Get input
		if((p = strchr(input, '\n')) != NULL){
			*p = '\0';				//Remove trailing \n if necessary
		}
	}
	string s = strdup(input);
	return s;
}

/**
	Remove leading & trailing spaces from a string
	@param s a string
	@return the given string, with leading & trailing spaces removed
*/
string strip(string s){
	if(s == NULL){							//Prevent errors
		return s;
	}
	int lead = 0, trail = 0;
	char* c = s;							//Focus pointer on first char
	while(*c == ' '){						//Look at beginning for spaces
		lead++;
		c++;
	}
	c = s+strlen(s)-1;						//Focus pointer on last char
	while(*c == ' '){						//Look at end for spaces
		trail++;
		c--;
	}
	return substring(s, lead, -trail);		//Use the # of each for a substring
}

/**
	Extract a substring from a given string.  Behaves similarly to python's
	string[from:to] substring notation.
	@param s the original string
	@param from the index of the first character to include
	@param to the index of the last character to include
	@return the portion of s between from and to
*/
string substring(string s, int from, int to){
	if(s == NULL){							//Prevent errors
		return s;
	}
	int firstChar, lastChar, len;
	
	if(from < 0){							//From the end, backwards
		firstChar = strlen(s) + from;
	} else if(from >= strlen(s)){			//Outside of string bounds
		return NULL;
	} else {								//From user provided index
		firstChar = from;
	}
	
	if(to == 0){							//To the end
		lastChar = strlen(s);
	} else if(to < 0){						//To x chars before the end
		lastChar = strlen(s) + to;
	} else if(to > (strlen(s))){			//Preserve original string end
		lastChar = strlen(s);
	} else {								//To user provided index
		lastChar = to;
	}

	len = lastChar - firstChar;
	char building[len+1], *p;				//Prepare a new char array
	building[len] = '\0';
	p = building;
	
	int i;
	for(i = firstChar; i < lastChar; i++){
		*p++ = s[i];						//Populate the new char array
	}
	
	string news = strdup(building);
	return news;							//Return the new string
}
