/*--------------------------------------------------------------------*/
/* symTable_client.c                                                  */
/* A client of the Symbol Table Module                                */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symTable.h"

int main(int argc, char *argv[])
{
   char *key[4] = { "Einstein", "Newton", "Bohr", "Curie" };
   char cvalue, char_value[4] = { 'e', 'n', 'b', 'c' };
   int  ivalue, int_value[4] = { 10, 20, 30, 40 };
   float fvalue, float_value[4] = { 1.11, 2.22, 3.33, 4.44 };
   double darray[2], double_array[4][2] = { { 1.11, 2.22 },
   					{ 3.33, 4.44 }, { 5.55, 6.66 }, { 7.77, 8.88 } };

   int i, rv;

   SymTable_T charTable, intTable, floatTable, double_arrayTable;

   /* Create and insert character values into symbol table. */
   printf("\nCreating symbol table with character values\n");
   printf("-------------------------------------------\n");
   charTable = symTable_create();
   if (charTable == NULL) {
      fprintf(stderr, "Cannot create character table\n");
      return 1;
   }
   for (i = 0; i < 4; i++) {
      printf("Insert <%s, %c>\n", key[i], char_value[i]);
      rv = symTable_insert(charTable, key[i], &char_value[i], sizeof(char));
	  if (!rv) {
	     fprintf(stderr, "Insert failed for %s\n", key[i]);
		 return 1;
	  }
   }

   /* Create and insert integer values into symbol table. */
   printf("\nCreating symbol table with integer values\n");
   printf("-----------------------------------------\n");
   intTable = symTable_create();
   if (intTable == NULL) {
      fprintf(stderr, "Cannot create integer table\n");
      return 1;
   }
   for (i = 0; i < 4; i++) {
      printf("Insert <%s, %d>\n", key[i], int_value[i]);
      rv = symTable_insert(intTable, key[i], &int_value[i], sizeof(int));
   	  if (!rv) {
   	     fprintf(stderr, "Insert failed for %s\n", key[i]);
   		 return 1;
   	  }
   }

   /* Create and insert float values into symbol table. */
   printf("\nCreating symbol table with float values\n");
   printf("---------------------------------------\n");
   floatTable = symTable_create();
   if (floatTable == NULL) {
      fprintf(stderr, "Cannot create float table\n");
      return 1;
   }
   for (i = 0; i < 4; i++) {
      printf("Insert <%s, %g>\n", key[i], float_value[i]);
      rv = symTable_insert(floatTable, key[i], &float_value[i], sizeof(float));
      if (!rv) {
   	     fprintf(stderr, "Insert failed for %s\n", key[i]);
         return 1;
      }
   }

   /* Create and insert an array two double values into symbol table. */
   printf("\nCreating symbol table with arrays of two double values\n");
   printf("------------------------------------------------------\n");
   double_arrayTable = symTable_create();
   if (double_arrayTable == NULL) {
      fprintf(stderr, "Cannot create double array table\n");
      return 1;
   }
   for (i = 0; i < 4; i++) {
      printf("Insert <%s, [%g, %g]>\n", key[i], double_array[i][0], double_array[i][1]);
      rv = symTable_insert(double_arrayTable, key[i], &double_array[i][0], 2*sizeof(double));
      if (!rv) {
   	     fprintf(stderr, "Insert failed for %s\n", key[i]);
         return 1;
      }
   }

   /* Search and print <key, value> pairs in character table, then delete */
   printf("\nSearching symbol table with character values\n");
   printf("--------------------------------------------\n");
   for (i = 0; i < 4; i++) {
      rv = symTable_search(charTable, key[i], &cvalue, sizeof(char));
	  if (rv) {
	     printf("%s found, value = %c: now deleting\n", key[i], cvalue);
	     if (!symTable_delete(charTable, key[i])) {
	        fprintf(stderr, "Delete failed for %s\n", key[i]);
	        return 1;
	     }
      }
	  else  {
		 fprintf(stderr, "Search failed for %s\n", key[i]);
		 return 1;
      }
   }

   /* Search and print <key, value> pairs in integer table, then delete */
   printf("\nSearching symbol table with integer values\n");
   printf("------------------------------------------\n");
   for (i = 0; i < 4; i++) {
      rv = symTable_search(intTable, key[i], &ivalue, sizeof(int));
	  if (rv) {
	     printf("%s found, value = %d: now deleting\n", key[i], ivalue);
		 if (!symTable_delete(intTable, key[i])) {
		    fprintf(stderr, "Delete failed for %s\n", key[i]);
	   	    return 1;
		 }
      }
	  else  {
	     fprintf(stderr, "Search failed for %s\n", key[i]);
	     return 1;
	  }
   }

   /* Search and print <key, value> pairs in float table, then delete */
   printf("\nSearching symbol table with float values\n");
   printf("----------------------------------------\n");
   for (i = 0; i < 4; i++) {
      rv = symTable_search(floatTable, key[i], &fvalue, sizeof(float));
   	  if (rv) {
   	     printf("%s found, value = %g: now deleting\n", key[i], fvalue);
   		 if (!symTable_delete(floatTable, key[i])) {
   		    fprintf(stderr, "Delete failed for %s\n", key[i]);
   	   	    return 1;
   		 }
      }
   	  else  {
   	     fprintf(stderr, "Search failed for %s\n", key[i]);
   	     return 1;
   	  }
   }

   /* Search and print <key, value> pairs in double array table, then delete */
   printf("\nSearching symbol table with arrays of two double values\n");
   printf("-------------------------------------------------------\n");
   for (i = 0; i < 4; i++) {
      rv = symTable_search(double_arrayTable, key[i], &darray, 2*sizeof(double));
	  if (rv) {
	     printf("%s found, value = [%g, %g]: now deleting\n", key[i], darray[0], darray[1]);
	     if (!symTable_delete(double_arrayTable, key[i])) {
	        fprintf(stderr, "Delete failed for %s\n", key[i]);
	   	   	return 1;
	     }
      }
	  else  {
	     fprintf(stderr, "Search failed for %s\n", key[i]);
	   	 return 1;
	  }
   }

   symTable_destroy(charTable);
   symTable_destroy(intTable);
   symTable_destroy(floatTable);
   symTable_destroy(double_arrayTable);
   return 0;
}

