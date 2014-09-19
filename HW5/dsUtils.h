/**
	Misc. useful utilities & definitions
	Author: Douglas Skrypa
	Date: 2014.04.27
*/

#include <stdlib.h>					//Memory functions; needed for size_t type

typedef char* string;				//Define the string type for ease of use

/*	Vector functions	===================================================	*/
/*
	Vector is an incomplete implementation of an expandable array.  It is
	incomplete in that it does not have any functions for removing individual
	items from the array.  That functionality was not necessary for the program
	that this was designed to be used in.
*/

/**
	A vector that stores a resizable array of strings
*/
typedef struct Vector{
	string* contents;
	size_t elements;
	size_t capacity;
} vector;

/**
	Initialize memory for a vector
	@return a pointer to the newly created vector
*/
vector* createVector();

/**
	Get the element from the given vector at the given index
	@param v a vector
	@param index the index of the desired item
	@return the string at the given index
*/
string getElement(vector* v, int index);

/**
	Get the number of elements in the given vector
	@param v a vector
	@return the number of strings stored in the given vector
*/
int getSize(vector* v);

/**
	Add a string to the given vector
	@param v a vector
	@param s a string
	@return 1 on success, 0 on failure (insufficient memory)
*/
unsigned insertElement(vector* v, string s);

/**
	Increase the capacity of the given vector by 1
	@param v a vector
	@return 1 on success, 0 on failure (insufficient memory)
*/
unsigned expandVector(vector* v);

/**
	Release memory allocated to items in the given vector
	@param v a vector
*/
void freeVector(vector* v);

/*	Other functions	=======================================================	*/

/**
	Retrieve a string of user input from stdin
	@return a string of user input
*/
string getInput();

/**
	Remove leading & trailing spaces from a string
	@param s a string
	@return the given string, with leading & trailing spaces removed
*/
string strip(string s);

/**
	Extract a substring from a given string.  Behaves similarly to python's
	string[from:to] substring notation.
	@param s the original string
	@param from the index of the first character to include
	@param to the index of the last character to include
	@return the portion of s between from and to
*/
string substring(string s, int from, int to);
