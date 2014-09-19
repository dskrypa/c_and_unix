/**
	Homework 5
	geoDistance
	Author: Douglas Skrypa
	Date: 2014.04.27
*/

#include <stdio.h>
#include <stdlib.h>				//Memory functions
#include <string.h>				//String functions
#include "geoDistance.h"		//Geographical distance calculation functions
#include "symTable.h"			//Symbolic table definition & functions
#include "dsUtils.h"			//Misc commonly needed utilities that I wrote

//Function Prototypes
void commandHelp();
vector* extractArgs();
unsigned initTable(const string);
unsigned cityLoc(const string);
unsigned cityDist(const string, const string);
unsigned cityAdd(const string, double, double);
unsigned cityRem(const string);

//Variables
const string delims = " ,";
SymTable_T cities;

/**
	Main loop for user interface.
	@param argc the number of arguments provided at runtime
	@param argv the arguments provided at runtime
	@return 0 after executing
*/
int main(int argc, string* argv){
	printf("Geological Distance Calculator\n");
	if(argc < 2){							//Make sure a file name is provided
		printf("Invalid input.  Please provide a file name.\n");
		return -1;
	}
	
	unsigned canRun = 1;					//Variable to control main loop
	cities = symTable_create();				//Create the cities table
	if(!initTable(argv[1])){
		canRun = 0;							//Quit if file could not be read
	}
	commandHelp();							//Print available user commands

	while(canRun == 1){
		printf(" » ");
		char* input = getInput();
		if(strlen(input) == 0){
			free(input);
			continue;
		}
		//Tokenize user input for processing a command + args
		string t = strtok(input, delims);
		string command = strdup(t);
		
		//Choose a function based on user input
		if((strcasecmp(command, "quit") == 0) ||
			(strcasecmp(command, "exit") == 0))
		{
			canRun = 0;
		} else if(strcasecmp(command, "help") == 0){
			commandHelp();
		} else if(strcasecmp(command, "locate") == 0){	//1 arg
			string args = strip(strtok(NULL, ""));
			cityLoc(args);
			free(args);
		} else if(strcasecmp(command, "dist") == 0){	//2 args
			string arg1 = strip(strtok(NULL, ","));
			string arg2 = strip(strtok(NULL, ""));
			cityDist(arg1, arg2);
			free(arg1);
			free(arg2);
		} else if(strcasecmp(command, "add") == 0){		//3 args
			string city = strip(strtok(NULL, ","));
			string lats = strip(strtok(NULL, ","));
			string lons = strip(strtok(NULL, ","));
			if((city == NULL) || (lats == NULL) || (lons == NULL)){
				printf("Error: Insufficient arguments provided: %s, %s, %s\n",
						city, lats, lons);
			} else {
				double lat, lon;			//Convert lat & lon to double
				sscanf(lats, "%lf", &lat);
				sscanf(lons, "%lf", &lon);
				cityAdd(city, lat, lon);
			}
			free(city);
			free(lats);
			free(lons);
		} else if(strcasecmp(command, "remove") == 0){	//1 arg
			string args = strip(strtok(NULL, ""));
			cityRem(args);
			free(args);
		} else{
			printf("Invalid command: %s\n", input);
		}
		//Release memory
		free(input);
		free(command);
	}
	symTable_destroy(cities);
	return 0;
}

/**
	Initialize the table of cities & their coordinates from the file at the
	given location.
	@param fileLoc the path to the file that should be loaded
	@return 1 on success, otherwise 0
*/
unsigned initTable(const string fileLoc){
	FILE* f;
	if((f = fopen(fileLoc, "r")) == NULL){
		printf("Unable to open file: %s\n", fileLoc);
		return 0;							//Unreadable / nonexistant -> abort
	}
	printf("Loading file: %s\n", fileLoc);
	
	string line = NULL;						//Init string to hold each line
	size_t len = 0;
	while(getline(&line, &len, f) != -1){	//Iterate through each line of file
		double lat, lon;
		string city = strtok(line, ",");	//Separate city,lat,lon
		string lats = strtok(NULL, ",");
		string lons = strtok(NULL, ",");
		sscanf(lats, "%lf", &lat);			//Convert lat & lon to double
		sscanf(lons, "%lf", &lon);
		double coords[2] = {lat, lon};		//Put in array for storage in table
		symTable_insert(cities, city, &coords, sizeof(coords));
	}
	free(line);								//Free memory allocated for input
	fclose(f);								//Close the file
	return 1;								//Return 1 for success
}

/**
	Search the table for the given city; display city coordinates if it exists.
	@param city the name of a city
	@return 1 if it was found successfully, 0 otherwise
*/
unsigned cityLoc(const string city){
	if(city == NULL){						//Make sure args were provided
		printf("Error: Insufficient arguments provided.\n");
		return 0;
	}
	double* coords;							//Allocate memory for coord array
	coords = (double*)malloc(2*sizeof(double));
	//Search the symTable for the given city
	int result = symTable_search(cities, city, coords, 2*sizeof(double));
	if(result == 0){
		printf("Error: Could not locate '%s'\n", city);
		free(coords);
		return 0;
	}
	//Print the city's information
	printf("%s:\tlatitude = %lf, longitude = %lf\n",
			city, coords[0], coords[1]);
	free(coords);							//Free memory from coord array
	return 1;
}

/**
	Calculate the distance between two cities based on their coordinates
	@param city1 a city
	@param city2 another city
	@return 1 if found successfully and distance was calculated, 0 otherwise
*/
unsigned cityDist(const string city1, const string city2){
	if((city1 == NULL) || (city2 == NULL)){	//Make sure args were provided
		printf("Error: Insufficient arguments proivided: %s, %s\n",
				city1, city2);
		return 0;
	}
	//Allocate memory for coordinate arrays, and search table for the cities
	double* coords1;
	coords1 = (double*)malloc(2*sizeof(double));
	int result1 = symTable_search(cities, city1, coords1, 2*sizeof(double));
	double* coords2;
	coords2 = (double*)malloc(2*sizeof(double));
	int result2 = symTable_search(cities, city2, coords2, 2*sizeof(double));
	if((result1 == 0) || (result2 == 0)){
		if(result1 == 0){
			printf("Error: Could not locate '%s'\n", city1);
		}
		if(result2 == 0){
			printf("Error: Could not locate '%s'\n", city2);
		}
		free(coords1);						//Free memory allocated for coords
		free(coords2);
		return 0;							//Unable to find one or more city
	}
	//Print each city's information
	printf("%s:\tlatitude = %lf, longitude = %lf\n",
			city1, coords1[0], coords1[1]);
	printf("%s:\tlatitude = %lf, longitude = %lf\n",
			city2, coords2[0], coords2[1]);
	//Calculate the distance between the cities
	double dist = geodistance(coords1[0], coords1[1], coords2[0], coords2[1]);
	printf("Distance between %s and %s: %lf miles\n", city1, city2, dist);
	free(coords1);							//Free memory allocated for coords
	free(coords2);
	return 1;
}

/**
	Add a new city to the table of cities and their coordinates.
	@param city the name of the new city
	@param latitude the new city's latitude
	@param longitude the new city's longitude
	@return 1 if the city was successfully added, 0 otherwise
*/
unsigned cityAdd(const string city, double latitude, double longitude){
	if(city == NULL){						//Make sure args were provided
		printf("Error: Insufficient arguments proivided: %s, %lf, %lf\n",
				city, latitude, longitude);
		return 0;
	}
	
	//Put latitude & longitude in an array for storage in table
	double coords[2] = {latitude, longitude};
	int result = symTable_insert(cities, city, &coords, sizeof(coords));
	if(result == 0){
		printf("Error: Unable to add '%s' to table.\n", city);
		return 0;							//Already existed / some other error
	}
	return 1;								//Successfully added city to table
}

/**
	Remove a city from the table of cities and their coordinates
	@param city the name of the city to be removed
	@return 1 if the city was found and successfully removed, 0 otherwise
*/
unsigned cityRem(const string city){
	if(city == NULL){						//Make sure an arg was provided
		printf("Error: Insufficient arguments proivided: %s\n", city);
		return 0;
	}
	int result = symTable_delete(cities, city);
	if(result == 0){
		printf("Error: Unable to delete '%s' from the table.\n", city);
		return 0;							//Wasn't in table or couldn't remove
	}
	return 1;								//Was in table & removed
}

/**
	Converts remaining user input into a vector
	@return a vector containing the args from user input separated by spaces
*/
vector* extractArgs(){
	vector* args = createVector();
	string t;
	for(t = strtok(NULL, delims); t != NULL; t = strtok(NULL, delims)){
		insertElement(args, t);
	}
	return args;
}

/**
	Prints the possible commands that can be used
*/
void commandHelp(){
	printf("==============================\nAvailable Commands:\n");
	printf("locate <city>\t\t\tSearch for & display <city>'s location\n");
	printf("dist <city1>,<city2>\t\tCompute the distance between <city1> & <city2>\n");
	printf("add <city>,<lat>,<lon>\t\tAdd a new city with the given location\n");
	printf("remove <city>\t\t\tRemove city from list\n");
	printf("quit\n==============================\n");
}
