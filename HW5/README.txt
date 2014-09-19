C & Unix Systems, Homework 5
Douglas Skrypa
2014.04.27

Files included:
cities.dat    dsUtils.h      geoDistRunner.c  makefile2   symTable_client.c
commands.txt  geoDistance.c  makefile         README.txt  symTable.h
dsUtils.c     geoDistance.h  makefile1        symTable.c

==============================
Compile & Run:

Working directory: DouglasSkrypa_Homework5/
Commands to compile:

make 1
make 2

Alternate commands to compile:

make -f makefile1
make -f makefile2

Commands to run:

./hw5p1
./hw5p2 cities.dat

==============================

For the sample run shown in the assignment pdf:
cat commands.txt | ./hw5p2 cities.dat


Part 1 (makefile1): symTable.h, symTable.c, symTable_client.c

To make the symbolic table, I adapted the code that I had from the previous
assignment's hash table.  I created a SymNode struct to store key, value pairs,
as well as a pointer to a next item for using them in a linked list.  I
implemented the SymTable struct as containing a pointer to an array of SymNodes,
and storing its table size and the number of elements currently in the table.

Most functions from the hash table assignment could be recycled for the SymTable
implementation because the basic functionality is the same in both.  The only
difference was that the hash table had to be gotten from the SymTable in each
function instead of having it provided as a function parameter.  After that,
there were just a few small changes to deal with a different value type than
what was used in the hash table.  


Part 2 (makefile2): 

dsUtils (dsUtils.c, dsUtils.h):
A collection of utilities that I have found useful to have in multiple programs
written in c.  At the moment, it contains a partial implementation of vector
for storing an expandable array.  It also contains a function for removing
leading and trailing spaces from a string, and for extracting a substring.
Finally, it contains a function for simplifying the way user input is collected
from stdin by hiding the eccentricities of how to collect the desired string
from fgets, and simply returns a string of user input from stdin.

symTable (symTable.c, symTable.h):
The SymTable implementation that was written in part 1.

geoDistance (geoDistance.c, geoDistance.h):
Contains functions for converting between miles and kilometers, and between
radians and degrees, as well as the implementation of the geoDistance function
for finding the distance between 2 points on Earth based on their latitude and
longitude.

geoDistRunner (geoDistRunner.c):
Contains the main loop that deals with user input for interacting with the table
of cities and their coordinates.  I broke it down to having separate functions
for each of the different user commands, and one more function for processing
the file that is used to provide the initial list of cities and their
coordinates. 


Overall:
Everything compiles and runs properly, and there are no memory leaks according
to valgrind.  There is only one error that I get consistently, and is only
visible when running valgrind:

	Conditional jump or move depends on uninitialised value(s)

I assume this is just because of using the behavior that a pointer/variable that
is not given a value will default to NULL instead of explicitly setting
pointers/variables to NULL if they do not need a value right away.  The one time
that I discovered that it was unsafe to do so was when dealing with pointers to
the next element in a linked list (on the previous assignment).  The problem was
resolved by explitly pointing the "next" element to NULL when a new node is
initialized.
