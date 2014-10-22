// group: Travis Seville, Georgii Tkachuk
/* symtab.h
 *
 * Holds function definitions for the symbol table. The symbol table
 * is implemented as a global hash table that contains local symbol
 * tables for each function
 */

#ifndef SYMTAB_H
#define SYMTAB_H

#include "shared.h"
#include "usrdef.h"
#include "rulefuncs.h"
#include <stdlib.h>
#include <string.h>


#define GLOBAL 0

#define CLASSTYPE 1
#define FUNCTIONTYPE 2
#define DATATYPE 3
#define VARIABLETYPE 4
#define VARIABLEARRAYTYPE 5

#define _INT 10
#define _REAL 11
#define _BOOL 12
#define _USER 13

/* ----------------------------------------------------------------
 * Function declarations
 * ----------------------------------------------------------------
 */

struct symbol
{
        char * name;	// the name of the symbol
        char * type;	// the type name
        int lineNumber; // the line number introduced
        int symbolType; // eg Class, Function, Data Type, Variable
        int typeIndex; // eg INT, REAL, BOOL, or USER (class declarations)
        struct variable_declaration_list_t * vdl; // this is for class tpye structural equivalence
        struct identifier_list_t * scope; // the scope the variable was introduced, linked list up to global
        struct symbol * next; // this is used for the hash just in case something hashes out the same
};


void symtab_init();
void symtab_print(int numOfTabs);
struct symbol * lookupSymbol(char * _name, int _symbolType, struct identifier_list_t * _scope);
unsigned hash(char *str);
char *dupstr(char *s);
struct identifier_list_t * global_scope;
struct identifier_list_t * cur_class_scopes;
struct identifier_list_t * lookup_scope_by_id(char * id);
int lookupType(char * _name);
struct symbol * addSymbol(char * _name, int _lineNumber, int _symbolType, int _typeIndex, char * _type,
                          struct variable_declaration_list_t * _vdl, struct identifier_list_t * scope);
struct symbol * addDataType(char * _name, int _lineNumber);
struct symbol * addVariable(char * _name, int _lineNumber, struct type_denoter_t * _type, struct identifier_list_t * _scope);
struct symbol * addFunction(char * _name, int _lineNumber, struct identifier_list_t * _scope);
struct symbol * addClass(char * _name, int _lineNumber, struct variable_declaration_list_t * _vdl, struct identifier_list_t * _scope);
void delend(struct identifier_list_t **l);
#endif
