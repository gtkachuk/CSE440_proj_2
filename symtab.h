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

#define NO_VALUE_NUMBER -99999 // hopefully this number doesnt come up!

#define NOTYPEFOUND -1

#define MAXSYMBOLS 701
#define HASHSIZE 701

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

struct variable_table
{
	struct variable_t * t[MAXSYMBOLS];
	int current_index;
};

struct expression_table
{
	struct expression_value_number_t * t[MAXSYMBOLS];
	int current_index;
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

struct label_t * labelForID(char * id);

int value_number_for_var(struct variable_table * vt, struct variable_t * v);
int value_for_constant_var(struct variable_table * cvt, struct variable_t * v);
int value_number_for_expression(struct expression_table * et, struct expression_value_number_t * e);

int add_new_variable(struct variable_table * vt, struct variable_t * v);
void add_variable(struct variable_table * vt, struct variable_t * v, int value_number);
void add_constant_variable(struct variable_table * vt, struct variable_t * v, int val);
int add_new_expression(struct expression_table * et, struct expression_value_number_t * e);
void add_expression(struct expression_table * et, struct expression_value_number_t * e, int value_number);
void add_label(struct label_t * l);

struct variable_t * get_variable(char * id);

void delete_value_number_for_var(struct variable_table * vt, struct variable_t * v);
void delete_value_for_constant_var(struct variable_table * cvt, struct variable_t * v);
void delete_value_number_for_expression(struct expression_table * et, struct expression_value_number_t * e);

void change_value_number_for_var(struct variable_table * vt, struct variable_t * v, int value_number);
void change_value_for_constant_var(struct variable_table * cvt, struct variable_t * v, int constant_value);
void change_value_number_for_expression(struct expression_table * et, struct expression_value_number_t * e, int value_number);

int index_for_expression(struct expression_table * et, struct expression_value_number_t * e);
void variable_table_print(struct variable_table * vt);
void constant_variable_table_print(struct variable_table * cvt);
void expression_table_print(struct expression_table * et);
void label_table_print();
#endif
