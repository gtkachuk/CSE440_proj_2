// group: Travis Seville, Georgii Tkachuk
/* symtab.c
 *
 * Implements the symbol table
 *
 * A flat symbol table is the root of all evil.
 */

#include "symtab.h"
#include "error.h"

#define NOTYPEFOUND -1

#define MAXSYMBOLS 701
#define HASHSIZE 701

int currentIndex = 10;

struct symbol * symbolHash[MAXSYMBOLS];

int current_variable = 0;
struct variable_t * variableTable[MAXSYMBOLS];

int current_expression = 0; 
struct op_code_t * expressionTable[MAXSYMBOLS];

int current_label = 0;
struct label_t * labelTable[MAXSYMBOLS];


int valueNumberForVar(struct variable_t * v)
{

}

int valueNumberIndexForVar(struct variable_t * v)
{

}

int valueNumberForExpression(struct op_code_t * e)
{

}

int valueNumberIndexForExpression(struct op_code_t * e)
{

}

int lineNumberForLabel(struct label_t * l)
{

}

struct label_t * labelForID(char * id)
{
	struct label_t * l;
	int i = 0;
	for (i; i < MAXSYMBOLS; i++)
	{
		l = labelTable[i];
		if (l == NULL) continue;
		if (strcasecmp(l->id, id) == 0)
		{
			return l;
		}
	}
	return NULL;
}

void addvariable(struct variable_t * v)
{
	variableTable[current_variable++] = v;
}

void addExpression(struct op_code_t * e)
{
	expressionTable[current_expression++] = e;
}

void addLabel(struct label_t * l)
{
	labelTable[current_label++] = l;
}

struct symbol * addSymbol(char * _name, int _lineNumber, int _symbolType, int _typeIndex, char * _type,
			  struct variable_declaration_list_t * _vdl, struct identifier_list_t * _scope)
{
	if ((strcasecmp(_scope->id, "NOSCOPE") == 0)) return NULL;
	struct identifier_list_t *temp = _scope;
	//if(DEBUG) printf("\n#################\nADDING: %s\nTYPE: %s SCOPE ID: %s\n-----------------------\n", _name, _type, temp->id);
	struct symbol *s;
	unsigned hashval;
	if ((s = lookupSymbol(_name, _symbolType, _scope)) == NULL)
	{
		s = (struct symbol *) malloc(sizeof(*s));
		if (s == NULL || (s->name = dupstr(_name)) == NULL)
		{
			return NULL;
		}
		hashval = hash(_name);
		s->name = _name;
		s->vdl = _vdl;
		s->lineNumber = _lineNumber;
		s->typeIndex = _typeIndex;
		if (_type != NULL)
		{
			s->type = dupstr(_type);
		}
		else
		{
			s->type = NULL;
		}
		s->symbolType = _symbolType;
		s->scope = _scope;
		s->next = symbolHash[hashval];
		symbolHash[hashval] = s;
		
	}
	else
	{
		switch (_symbolType)
		{
			case CLASSTYPE:
                //this doesn't pass for some reason
				//error_class_already_declared(_lineNumber, _name, s->lineNumber);
				break;
			case FUNCTIONTYPE:
				//error_function_already_declared(_lineNumber, _name, s->lineNumber);
				break;
			case DATATYPE:
				//error_type_already_defined(_lineNumber, _name, s->lineNumber);
				break;
			case VARIABLETYPE:
				//error_variable_already_declared(_lineNumber, _name, s->lineNumber);
                break;
		}
	}
	return s;
}

struct symbol * addDataType(char * _name, int _lineNumber)
{
	return addSymbol(_name, _lineNumber, DATATYPE, currentIndex++, _name, NULL, cur_class_scopes);
}

struct symbol * addVariable(char * _name, int _lineNumber, struct type_denoter_t * _type, struct identifier_list_t * _scope)
{
	struct symbol * s;
	//if (DEBUG) printf("IN ADD VARIABLE TYPE: %s TYPE CODE: %d", _type->name, _type->type);
	struct type_denoter_t  *tempType = _type;
//	if (tempType->type == TYPE_DENOTER_T_ARRAY_TYPE)
	{
		while (tempType->type == TYPE_DENOTER_T_ARRAY_TYPE)
		{
			tempType = tempType->data.at->td;
		}
		
	}
	if (tempType->type == TYPE_DENOTER_T_IDENTIFIER)
	{
		s = lookupSymbol(tempType->name, DATATYPE, _scope);
	}
	else if (tempType->type == TYPE_DENOTER_T_CLASS_TYPE)
	{
		s = lookupSymbol(tempType->name, CLASSTYPE, _scope);
	}
	int symbolType = NOTYPEFOUND;
	
	if (s != NULL)
	{
		symbolType = s->symbolType;
	}
	if (symbolType == NOTYPEFOUND)
	{
		//if (DEBUG) printf("\nTYPE NOT FOUND: %s\n", _type->name);
		error_type_not_defined(_lineNumber, _type->name);
		return NULL;
	}
	int varType = VARIABLETYPE;
	if (_type->type == TYPE_DENOTER_T_ARRAY_TYPE) varType = VARIABLEARRAYTYPE;
	return addSymbol(_name, _lineNumber, varType, s->typeIndex, s->name, NULL, _scope);
}

struct symbol * addFunction(char * _name, int _lineNumber, struct identifier_list_t * _scope)
{
	struct symbol * s;
	if ((s = lookupSymbol(_name, FUNCTIONTYPE, _scope)) != NULL)
	{
		//error_function_already_declared(_lineNumber, _name, s->lineNumber);
		return NULL;
	}
	return addSymbol(_name, _lineNumber, FUNCTIONTYPE, FUNCTIONTYPE, "function", NULL, _scope);
}

struct symbol * addClass(char * _name, int _lineNumber, struct variable_declaration_list_t * _vdl, struct identifier_list_t * _scope)
{
	struct symbol * s;
	if ((s = lookupSymbol(_name, CLASSTYPE, _scope)) != NULL)
	{
		error_class_already_declared(_lineNumber, _name, s->lineNumber);
		return NULL;
	}
	return addSymbol(_name, _lineNumber, CLASSTYPE, currentIndex++, "class", _vdl, _scope);
}

int lookupType(char * _name)
{
	if (strcasecmp(_name, PRIMITIVE_TYPE_NAME_INTEGER) == 0 ||
	    strcasecmp(_name, PRIMITIVE_TYPE_NAME_BOOLEAN) == 0 ||
	    strcasecmp(_name, PRIMITIVE_TYPE_NAME_POINTER) == 0)
		return TYPE_DENOTER_T_IDENTIFIER;
	else
		return TYPE_DENOTER_T_CLASS_TYPE;
}

int lookupTypeIndex(char * _name, struct identifier_list_t * _scope)
{
	struct symbol * s;
        int type = lookupType(_name);
	if (type == TYPE_DENOTER_T_IDENTIFIER)
        {
                s = lookupSymbol(_name, DATATYPE, _scope);
        }
        else if (type == TYPE_DENOTER_T_CLASS_TYPE)
        {
                s = lookupSymbol(_name, CLASSTYPE, _scope);
        }
	if (s == NULL) // if it is not a primitive or class it is a variable
	{
		s = lookupSymbol(_name, VARIABLETYPE, _scope);
	}
	return s->typeIndex;
}

int lookupInClass(char * _name, struct class_list_t * scope)
{
    
    
}

struct symbol * lookupSymbol(char * _name, int _symbolType, struct identifier_list_t * _scope)
{
    	struct symbol * s;
	struct identifier_list_t * tempScope;
	tempScope  = _scope;
	for (s = symbolHash[hash(_name)]; s != NULL; s = s->next)
	{
		if ((strcasecmp(_name, s->name) == 0) &&
		    (_symbolType == s->symbolType))
		{
			struct identifier_list_t * foundScope;
			foundScope = s->scope;
			//if (DEBUG) printf("\n\nIN LOOKUP LOOP for %s\n", foundScope->id);
			while (tempScope != NULL)
			{
				//if (DEBUG) printf("\n\n%s -=-=-= temp/found -=-=-= %s\n", tempScope->id, foundScope->id);
				if (strcasecmp(tempScope->id, foundScope->id) == 0)
				{
					return s;
				}
				tempScope = tempScope->next;
			}	
			//if (DEBUG) printf("OUT OF LOOKUP LOOP\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
		}
	}
    return NULL;
}

unsigned hash(char *str)
{ // this hashes the string str
    unsigned hashval;
    for (hashval = 0; *str != '\0'; str++)
      hashval = *str + 31 * hashval;
    return hashval % HASHSIZE;
}

char *dupstr(char *s)
{
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
    if (p != NULL)
       strcpy(p, s);
    return p;
}

/* ------------------------------------------------------------
 * Lookup class scope by id
 * ------------------------------------------------------------
 */
struct identifier_list_t * lookup_scope_by_id(char * id){
    struct identifier_list_t *scope_ptr = global_scope;
    while(scope_ptr->next != NULL){
        if(strcasecmp(scope_ptr->id, id) == 0)
            return scope_ptr;
        scope_ptr = scope_ptr->next;
    }
    //no such scope:
    return NULL;
}


/* ------------------------------------------------------------
 * Initializes the symbol table
 * ------------------------------------------------------------
 */
void symtab_init()
{
	cur_class_scopes = new_identifier_list();
	cur_class_scopes->id = "GLOBAL";
	addDataType(PRIMITIVE_TYPE_NAME_INTEGER, 0);
	addDataType(PRIMITIVE_TYPE_NAME_BOOLEAN, 0);
	addDataType("real", 0);		
	struct type_denoter_t * td = new_type_denoter();
	td->name = PRIMITIVE_TYPE_NAME_BOOLEAN;
	td->type = TYPE_DENOTER_T_IDENTIFIER;
	addVariable(BOOLEAN_VALUE_TRUE, 0, td, cur_class_scopes);
	addVariable(BOOLEAN_VALUE_FALSE, 0, td, cur_class_scopes);
	//G: not sure how to access global scope, so this is a pointer to global
	global_scope = cur_class_scopes;

}

char * convertType(int i)
{
	switch (i)
	{
		case CLASSTYPE:
			return "Class";
			break;
		case FUNCTIONTYPE:
			return "Function";
			break;
		case DATATYPE:
			return "Data type";
			break;
		case VARIABLETYPE:
			return "Varaible";
			break;
		case VARIABLEARRAYTYPE:
			return "Array";
			break;
		default:
			return "Unknown";
	}
}

/* ------------------------------------------------------------
* Prints the contents of the symbol table
* ------------------------------------------------------------
*/void symtab_print(int numOfTabs)
{
	int i;
	printf("\n\n\n%55s\t%15s\t%15s\t%15s\t%30s%8s\n", "Type : Name", "Hash Value", "Type Index", "Type", "Scope", "Line #");
	for (i = 0; i < MAXSYMBOLS; i++)
	{
		if (symbolHash[i] == NULL)
		{
			continue;
		}
		struct symbol * s = symbolHash[i];
		while (s != NULL)
		{
			printf("%20s : %35s\t%15d\t%15d\t%15s\t%30s%8d\n", s->type, s->name, hash(s->name), s->typeIndex,
								    convertType(s->symbolType), s->scope->id, s->lineNumber);
			s = s->next;
		}	
	}
	printf("\n\n\n");
}

void delend(struct identifier_list_t **l)
{
    if (!l || !(*l))
        return;
    if (!(*l)->next)
    {
        *l = NULL;
    }
    struct identifier_list_t *tmp;
    tmp=*l;
    while(tmp->next->next!=NULL)
    {
        tmp=tmp->next;
    }
    free(tmp->next);
    tmp->next = NULL;
    return;
}


