/*--------------------------------------------------------------------*/
/* symTable.h                                                         */
/* Generic Symbol Table Interface                                     */
/*--------------------------------------------------------------------*/

#ifndef SYMTABLE_H
#define SYMTABLE_H

/*--------------------------------------------------------------------*/

/* SymTable is a symbol table object pointed to by a pointer of type
   SymTable_T. SymTable stores <key, value> pairs. All keys in the
   table are distinct and are assumed to be character strings. The
   value associated with a key is an arbitrary object defined by the
   user that is passed to the ADT via a void pointer. In other words,
   the ADT is generic.

   A SymTable object creates copies of <key, value> pairs inserted
   into it; these copies are destroyed when the <key, value> pairs
   are deleted from the table or when the SymTable object itself
   is destroyed. */

typedef struct SymTable *SymTable_T;

/*--------------------------------------------------------------------*/

/* Return a new SymTable object or NULL if insufficient memory. */

SymTable_T symTable_create(void);

/*--------------------------------------------------------------------*/

/* Destroy the symbol table by freeing all the space allocated to it. */

void symTable_destroy(SymTable_T symTable);

/*--------------------------------------------------------------------*/

/* Return the number of <key, value> pairs stored in symbol table. */

int symTable_size(SymTable_T symTable);

/*--------------------------------------------------------------------*/

/* If key is not in the symbol table, insert it in the table and
   set its value to the object pointed to by value, whose size
   in bytes is valuesize. Return 1 if the key is successfully
   inserted. Return 0 if key is already in the table or it is
   not in the table but it could not be inserted because of
   insufficient memory. */

int symTable_insert(SymTable_T symTable, const char *key,
                    const void *value, size_t valuesize);

/*--------------------------------------------------------------------*/

/* Search for key in the symbol table. If it is in the table, copy
   its value to the object pointed to by value and return 1. If
   key is not in the table, return 0. */

int symTable_search(SymTable_T symTable, const char *key,
                    void *value, size_t valuesize);

/*--------------------------------------------------------------------*/

/* Delete key and its value from the table. Return 1 if key is in
   the table and was successfully deleted; otherwise, return 0. */

int symTable_delete(SymTable_T symTable, const char *key);

/*--------------------------------------------------------------------*/

#endif
