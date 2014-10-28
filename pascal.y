%{
// group: Travis Seville, Georgii Tkachuk
/*
 * grammar.y
 *
 * Pascal grammar in Yacc format, based originally on BNF given
 * in "Standard Pascal -- User Reference Manual", by Doug Cooper.
 * This in turn is the BNF given by the ANSI and ISO Pascal standards,
 * and so, is PUBLIC DOMAIN. The grammar is for ISO Level 0 Pascal.
 * The grammar has been massaged somewhat to make it LALR.
 */

#include "shared.h"
#include "rulefuncs.h"
#include "symtab.h"

int yylex(void);
void yyerror(const char *error);

extern char *yytext;		  /* yacc text variable */
extern int line_number;	   /* Holds the current line number; specified
				   in the lexer */

struct function_declaration_t *cur_function; 	/* points to the current function. Not sure if need this*/
extern struct identifier_list_t *global_scope;
extern struct identifier_list_t *cur_class_scopes;
/* points to the current class scope, starting with the current, each next is higher, up to global, this is held in symtab */
struct program_t *program;			/* points to our program */
extern int lookupTypeIndex(char * _name, struct identifier_list_t * _scope);
void dprint(char * str)
{
	if (DEBUG)
	{
		printf("\n\n-----------------%s---------------\n\n", str);
	}
}
%}

%token AND ARRAY ASSIGNMENT CLASS COLON COMMA DIGSEQ
%token DO DOT DOTDOT ELSE END EQUAL EXTENDS FUNCTION
%token GE GT IDENTIFIER IF LBRAC LE LPAREN LT MINUS MOD NEW NOT
%token NOTEQUAL OF OR PBEGIN PLUS PRINT PROGRAM RBRAC
%token RPAREN SEMICOLON SLASH STAR THEN
%token VAR WHILE GOTO


%type <program> program
%type <ph> program_heading
%type <ap> actual_parameter
%type <apl> actual_parameter_list
%type <op> addop
%type <at> array_type
%type <as> assignment_statement
%type <ad> attribute_designator
%type <e> boolean_expression
%type <cb> class_block
%type <ci> class_identification
%type <cl> class_list
%type <ss> compound_statement
%type <e> expression
%type <f> factor
%type <fpsl> formal_parameter_list
%type <fps> formal_parameter_section
%type <fpsl> formal_parameter_section_list
%type <fdl> func_declaration_list
%type <fb> function_block
%type <funcd> function_declaration
%type <fdes> function_designator
%type <fh> function_heading
%type <id> function_identification
%type <id> identifier
%type <idl> identifier_list
%type <is> if_statement
%type <e> index_expression
%type <iel> index_expression_list
%type <iv> indexed_variable
%type <md> method_designator
%type <op> mulop
%type <os> object_instantiation
%type <apl> params
%type <p> primary
%type <ps> print_statement
%type <r> range
%type <op> relop
%type <id> result_type
%type <i> sign
%type <se> simple_expression
%type <s> statement
%type <ss> statement_part
%type <ss> statement_sequence
%type <t> term
%type <tden> type_denoter
%type <un> unsigned_constant
%type <un> unsigned_integer
%type <un> unsigned_number
%type <fps> value_parameter_specification
%type <va> variable_access
%type <vd> variable_declaration
%type <vdl> variable_declaration_list
%type <vdl> variable_declaration_part
%type <fps> variable_parameter_specification
%type <gts> goto_statement
%type <ws> while_statement
%type <l> label


%union
{
	char *id;	
	int	*i;	
	int	op;	
	struct	actual_parameter_list_t	*apl;
	struct	actual_parameter_t	*ap;
	struct	array_type_t	*at;
  //struct  op_code_t* as;
	struct	assignment_statement_t	*as;
	struct	attribute_designator_t	*ad;
	struct	class_block_t	*cb;
	struct	class_identification_t	*ci;
	struct	class_list_t	*cl;
	struct	expression_t	*e;
	struct	factor_t	*f;
	struct	formal_parameter_section_list_t	*fpsl;
	struct	formal_parameter_section_t	*fps;
	struct	func_declaration_list_t	*fdl;
	struct	function_block_t	*fb;
	struct	function_declaration_t	*funcd;
	struct	function_declaration_t	*cur_function;
	struct	function_declaration_t	*cur_class;
	struct	function_designator_t	*fdes;
	struct	function_heading_t	*fh;
	struct  goto_statement_t *gts;
	struct	identifier_list_t	*idl;
	struct	if_statement_t	*is;
	struct	index_expression_list_t	*iel;
	struct	indexed_variable_t	*iv;
	struct  label_t *l;
	struct	method_designator_t	*md;
	struct	object_instantiation_t	*os;
	struct	primary_t	*p;
	struct	print_statement_t	*ps;
	struct	program_heading_t	*ph;
	struct	program_t	*program;
	struct	range_t	*r;
	struct	simple_expression_t	*se;
	struct	statement_sequence_t	*ss;
  //replaced statement_t with code_t for statement.
	struct	statement_t	*s;
	struct	term_t	*t;
	struct	type_denoter_t	*tden;
	struct	unsigned_number_t	*un;
	struct	variable_access_t	*va;
	struct	variable_declaration_list_t	*vdl;
	struct	variable_declaration_t	*vd;
	struct	while_statement_t	*ws;
}

%%

program : program_heading semicolon class_list DOT
	{
		//This was already created as global in the first section. We just need to allocate.
		//struct program_t * program = new_program();
		program = new_program();
		$$ = program;
		$$->ph = $1;
		$$->cl = $3;
	}
 ;

program_heading : PROGRAM identifier
	{
		struct program_heading_t * program_heading = new_program_heading();
		$$ = program_heading;
		$$->id = $2;
	}
 | PROGRAM identifier LPAREN identifier_list RPAREN
	{
		struct program_heading_t * program_heading = new_program_heading();
		$$ = program_heading;
		$$->id = $2;
		$$->il = $4;
	}
 ;

identifier_list : identifier_list comma identifier
	{
		struct identifier_list_t * identifier_list = new_identifier_list();
		$$ = identifier_list;
		$$->id = $3;
		$$->next = $1;
	}
 | identifier
	{
		struct identifier_list_t * identifier_list = new_identifier_list();
		$$ = identifier_list;
		$$->id = $1;
	}
 ;

class_list: class_list class_identification PBEGIN class_block END
	{
		struct class_list_t * class_list = new_class_list();
		$$ = class_list;
		$$->ci = $2;
		$$->cb = $4;
		$$->next = $1;
		// get rid of this class from current scopes
		cur_class_scopes = cur_class_scopes->next;
	}
 | class_identification PBEGIN class_block END
	{
		struct class_list_t * class_list = new_class_list();
		$$ = class_list;
		$$->ci = $1;
		$$->cb = $3;
		// get rid of this class from current scopes
		cur_class_scopes = cur_class_scopes->next;
	}
 ;

class_identification : CLASS identifier
	{
		struct class_identification_t * class_identification = new_class_identification();
		$$ = class_identification;
		$$->id = $2;
        $$->line_number = line_number;
		//if (DEBUG) printf("\nIN THE BASE CLASS THING: %s\n",cur_class_scopes->id);
		struct symbol *s = addClass($$->id, line_number, NULL, cur_class_scopes);
		//set this cl as current scope. We can refer to this from other productions.
		if (s != NULL)
		{
			//if (DEBUG) printf("\nADDING %s TO CUR CLASS SCOPES.\n", $2);
			add_to_identifier_list_front(&cur_class_scopes, $2);
		}
		else
		{
			add_to_identifier_list_front(&cur_class_scopes, "NOSCOPE");
		}
	}
| CLASS identifier EXTENDS identifier
	{
		struct class_identification_t * class_identification = new_class_identification();
		$$ = class_identification;
		$$->id = $2;
		$$->extend = $4;
        $$->line_number = line_number;
		if (DEBUG) printf("\nIN THE extend CLASS THING: %s\nLine number: %d",cur_class_scopes->id, line_number);
		struct symbol * temp;
		if ((temp = lookupSymbol($4, CLASSTYPE,  cur_class_scopes)) == NULL)
		{
			error_extending_missing_class(line_number, $2, $4);
		}

		struct symbol *s = addClass($$->id, line_number, NULL, cur_class_scopes);
                //set this cl as current scope. We can refer to this from other productions.
                if (s != NULL)
                {
                        //if (DEBUG) printf("\nADDING %s TO CUR CLASS SCOPES.\n", $2);
                        add_to_identifier_list_front(&cur_class_scopes, $2);
                }
                else
                {
                        add_to_identifier_list_front(&cur_class_scopes, "NOSCOPE");
                }
	}
;

class_block:
 variable_declaration_part
 func_declaration_list
	{
		struct class_block_t * class_block = new_class_block();
		$$ = class_block;
		$$->vdl = $1;
		$$->fdl = $2;
	}
 ;

type_denoter : array_type
	{
		struct type_denoter_t * type_denoter = new_type_denoter();
		$$ = type_denoter;
		$$->type = TYPE_DENOTER_T_ARRAY_TYPE;
		$$->data.at = $1;
		$$->name = $$->data.at->td->name;
	}
 | identifier
	{
		struct type_denoter_t * type_denoter = new_type_denoter();
		$$ = type_denoter;
		// this may need to get changed. id may refer to a class, how to handle
		$$->type = lookupType($1);
		$$->name = $1;
	}
 ;

array_type : ARRAY LBRAC range RBRAC OF type_denoter
	{
		struct array_type_t * array_type = new_array_type();
		$$ = array_type;
		$$->r = $3;
		$$->td = $6;
	}
 ;

range : unsigned_integer DOTDOT unsigned_integer
	{
		struct range_t * range = new_range();
		$$ = range;
		$$->min = $1;
		$$->max = $3;
	}
 ;

variable_declaration_part : VAR variable_declaration_list semicolon
	{
		$$ = $2;
	}
 |
	{
        $$ = NULL; 
	}
 ;

variable_declaration_list : variable_declaration_list semicolon variable_declaration
	{
		struct variable_declaration_list_t * variable_declaration_list = new_variable_declaration_list();
		$$ = variable_declaration_list;
		$$->next = $1;
		$$->vd = $3;
	}
 | variable_declaration
	{
		struct variable_declaration_list_t * variable_declaration_list = new_variable_declaration_list();
		$$ = variable_declaration_list;
		$$->vd = $1;
	}

 ;

variable_declaration : identifier_list COLON type_denoter
	{
        struct variable_declaration_t * variable_declaration = new_variable_declaration();
        $$ = variable_declaration;
		$$->il = $1;
		$$->tden = $3;
		$$->line_number = line_number;
		struct identifier_list_t * _il = $1;
		while(_il != NULL)
		{
			//if (DEBUG) printf("\nADDING %s : TYPE: %s SCOPE: %s \n\n", _il->id, $3->name, cur_class_scopes->id);
			if (strcasecmp(_il->id, "True") == 0)
				error_variable_name_invalid(line_number, _il->id);
			if (strcasecmp(_il->id, "False") == 0)
				error_variable_name_invalid(line_number, _il->id);
			addVariable(_il->id, line_number, $3, cur_class_scopes);
			_il = _il->next;
		}
	}
 ;

func_declaration_list : func_declaration_list semicolon function_declaration
	{
		struct func_declaration_list_t * func_declaration_list = new_func_declaration_list();
		$$ = func_declaration_list;
		$$->next = $1;
		$$->fd = $3;
	}
 | function_declaration
	{
		struct func_declaration_list_t * func_declaration_list = new_func_declaration_list();
		$$ = func_declaration_list;
		$$->fd = $1;
	}
 |
	{
		$$= NULL;
	}
 ;

formal_parameter_list : LPAREN formal_parameter_section_list RPAREN 
	{
        $$ = $2;
	}
;
formal_parameter_section_list : formal_parameter_section_list semicolon formal_parameter_section
	{
		struct formal_parameter_section_list_t * formal_parameter_section_list = new_formal_parameter_section_list();
		$$ = formal_parameter_section_list;
		$$->fps = $3;
		$$->next = $1;
	}
 | formal_parameter_section
	{
		struct formal_parameter_section_list_t * formal_parameter_section_list = new_formal_parameter_section_list();
		$$ = formal_parameter_section_list;
		$$->fps = $1;
	}
 ;

formal_parameter_section : value_parameter_specification
	{
		$$ = $1;
	}

 | variable_parameter_specification
	{
		$$ = $1;
	}
 ;

value_parameter_specification : identifier_list COLON identifier
	{
		//Not sure where to fill this one in. 
		struct formal_parameter_section_t * formal_parameter_section = new_formal_parameter_section();
		$$ = formal_parameter_section;
		$$->il = $1;
		$$->is_var = 0; //it's a value
		$$->id = $3;
   	}
 ;

variable_parameter_specification : VAR identifier_list COLON identifier
	{
		struct formal_parameter_section_t * formal_parameter_section = new_formal_parameter_section();
		$$ = formal_parameter_section;
		$$->il = $2;
		$$->is_var = 1; //it's a variable
		$$->id = $4;	
	}
 ;


function_declaration : function_identification semicolon function_block
	{
		struct function_declaration_t * function_declaration = new_function_declaration();
        struct function_heading_t * fh = new_function_heading();
		$$ = function_declaration;
        $$->fh = fh;
        $$->fh->id = $1;
			//what is function_identification?	   the name of the function when there is no return type 
		$$->fb = $3;
		$$->line_number = line_number;

	}
 | function_heading semicolon function_block
	{
		struct function_declaration_t * function_declaration = new_function_declaration();
		$$ = function_declaration;
		$$->fh = $1;
		$$->fb = $3;
		$$->line_number = line_number;
	}
 ;

function_heading : FUNCTION identifier COLON result_type
	{
		struct function_heading_t * function_heading = new_function_heading();
		$$ = function_heading;
		$$->id = $2;
		$$->res = $4;
	}
 | FUNCTION identifier formal_parameter_list COLON result_type
	{
		struct function_heading_t * function_heading = new_function_heading();
		$$ = function_heading;
		$$->id = $2;
		$$->fpsl = $3;
		$$->res = $5;
}
 ;

result_type : identifier ;


function_identification : FUNCTION identifier
	{
		$$ = $2;
		addFunction($$, line_number, cur_class_scopes);
	}
;

function_block : 
  variable_declaration_part
  statement_part
	{
		struct function_block_t * function_block = new_function_block();
		$$ = function_block;
		$$->vdl = $1;
		$$->ss = $2;
	}
;

statement_part : compound_statement
 ;

compound_statement : PBEGIN statement_sequence END
	{
        $$ = $2;
	}
 ;

statement_sequence : statement
	{
		struct statement_sequence_t * statement_sequence = new_statement_sequence();
		$$ = statement_sequence;
		$$->s = $1;
		$$->code = $1->code;
	}
 | statement semicolon statement_sequence
	{
		struct statement_sequence_t * statement_sequence = new_statement_sequence(); 
		$$ = statement_sequence;
		$$->s = $1;
		$$->next = $3;
    $$->code = $1->code;
    
	}
 | label statement_sequence
    {
		printf("IN SS\n\n");
		struct statement_sequence_t * statement_sequence = new_statement_sequence();
		$$ = statement_sequence;
		$$->next = $2;
		$$->s = new_statement();
		$$->s->type = STATEMENT_T_LABEL;
		$$->s->data.l = $1;
		// add label next_ss here
    }
;

statement: assignment_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		// this needs to be assigned a type, and handle the struct
		$$->type = STATEMENT_T_ASSIGNMENT;
		$$->data.as = $1;
		$$->line_number = line_number;
		$$->code = $1->code;
	}
	| compound_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		$$->type = STATEMENT_T_SEQUENCE;
		$$->data.ss = $1;
		$$->line_number = line_number;
		//$$->code = $1->code;
	}
	| if_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		$$->type = STATEMENT_T_IF;
		$$->data.is = $1;
		$$->line_number = line_number;
		$$->code = $1->code;

	}
	| while_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		$$->type = STATEMENT_T_WHILE;
		$$->data.ws = $1;
		$$->line_number = line_number;
		$$->code = $1->code;
	}
	| print_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		$$->type = STATEMENT_T_PRINT;
		$$->data.ps = $1;
		$$->line_number = line_number;
		$$->code = $1->code;
	}
	| goto_statement
	{
		struct statement_t * statement = new_statement();
		$$ = statement;
		$$->type = STATEMENT_T_GOTO;
		$$->data.gs = $1;
		$$->code = $1->code;
	}
;

while_statement: WHILE boolean_expression DO compound_statement
	{
		struct while_statement_t * while_statement = new_while_statement();
		$$ = while_statement;
		$$->e = $2;
		$$->s = $4;
		$$->code = new_code();
		$$->code->type = T_WHILE_CODE;
		$$->code->t.if_code = new_if_code();
		struct code_t * temp_code = $2->code;
		GOTO_END_OF_LIST(temp_code);
		$$->code->t.if_code->v1 = temp_code->t.op_code->v1;
		$$->code->t.if_code->v2 = temp_code->t.op_code->v2;
		$$->code->t.if_code->op = temp_code->t.op_code->op;
		$$->code->t.if_code->true_target = $4->code;
		$$->code->t.if_code->false_target = NULL;
	}
;

if_statement : IF boolean_expression THEN compound_statement ELSE compound_statement
	{
		struct if_statement_t * if_statement = new_if_statement();
		$$ = if_statement;
		$$->e = $2;
		$$->s1 = $4;
		$$->s2 = $6;
		$$->code = new_code();
		$$->code->type = T_IF_CODE;
		$$->code->t.if_code = new_if_code();
		struct code_t * temp_code = $2->code;
		GOTO_END_OF_LIST(temp_code);
		$$->code->t.if_code->v1 = temp_code->t.op_code->v1;
		$$->code->t.if_code->v2 = temp_code->t.op_code->v2;
		$$->code->t.if_code->op = temp_code->t.op_code->op;
		$$->code->t.if_code->true_target = $4->code;
		$$->code->t.if_code->false_target = $6->code;
	}
 ;

assignment_statement : variable_access ASSIGNMENT expression
	{
		struct assignment_statement_t * assignment_statement = new_assignment_statement();
    
		$$ = assignment_statement;
		$$->va = $1;
		$$->e = $3;
		struct code_t * newcode = new_code();
		newcode->t.assign_code = new_assign_code();
		newcode->t.assign_code->assigned = new_variable();
		newcode->t.assign_code->assigned->id = $1->data.id;
		newcode->t.assign_code->v1 = $3->var;
		newcode->type = T_ASSIGN_CODE;
		if ($3->code != NULL)
		{
			struct code_t * temp_code = $3->code;
			GOTO_END_OF_LIST(temp_code);
			temp_code->next = newcode;
			$$->code = $3->code;
		}
		else
		{
			$$->code = newcode;
		}
		if (DEBUG) printf("HERE IN ASSIGNMENT_STATEMENT:VA %s = EXP %s\n", $1->data.id,
				newcode->t.op_code->v1->id);

	}
 | variable_access ASSIGNMENT object_instantiation
	{
    
		struct assignment_statement_t * assignment_statement = new_assignment_statement();
		$$ = assignment_statement;
    
	}
 ;

object_instantiation: NEW identifier
	{
		struct object_instantiation_t * object_instantiation = new_object_instantiation();
		$$ = object_instantiation;
		$$->id = $2;
	}
 | NEW identifier params
	{
		struct object_instantiation_t * object_instantiation = new_object_instantiation();
		$$ = object_instantiation;
		$$->id = $2;
		$$->apl = $3;
	}
;

print_statement : PRINT variable_access
	{
		struct print_statement_t * print_statement = new_print_statement();
		$$ = print_statement;
		$$->va = $2;
	}
;

goto_statement : GOTO identifier
  {
    struct goto_statement_t * goto_statement = new_goto_statement();
	$$ = goto_statement;
	$$->code = new_code();
	$$->code->type = T_GOTO_CODE;
	$$->code->t.goto_code = new_goto_code();
	$$->code->t.goto_code->label_id = $2;
	if (DEBUG) printf("HERE IN GOTO: %s\n", $2);
  }
;

label : identifier COLON statement
  {
	printf("IN LABEL\n");
    struct label_t* label = new_label();
    $$ = label;
    $$->id = $1;
	$$->line_number = line_number;
	addLabel(label);
  }
  | identifier COLON
  {
    struct label_t* label = new_label();
    $$ = label;
    $$->id = $1;
  }

;

variable_access : identifier
	{
		struct variable_access_t * variable_access = new_variable_access();
		$$ = variable_access;
	        $$->type = 1;
		$$->data.id = $1;

		$$->var = new_variable();
		$$->var->id = $1;
		$$->var->type = VARIABLE_TYPE;

		$$->type = VARIABLE_ACCESS_T_IDENTIFIER;
		struct symbol * s = lookupSymbol($1, VARIABLETYPE, cur_class_scopes);
		$$->expr = new_expression_data();
		if (s == NULL) s = lookupSymbol($1, VARIABLEARRAYTYPE, cur_class_scopes);
		if (s != NULL)
		{
			$$->expr->type = s->type;
		}
		else
		{
//			error_variable_not_declared(line_number, $1);
		}
	}
 | indexed_variable
	{
		struct variable_access_t * variable_access = new_variable_access();
		$$ = variable_access;
	        $$->type = 2;
		$$->data.iv = $1;
		$$->type = VARIABLE_ACCESS_T_INDEXED_VARIABLE;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
       
	}
 | attribute_designator
	{
		struct variable_access_t * variable_access = new_variable_access();
		$$ = variable_access;
	        $$->type = 3;
		$$->data.ad = $1;
		$$->type = VARIABLE_ACCESS_T_ATTRIBUTE_DESIGNATOR;
		$$->expr = new_expression_data();
		$$->expr->type = $1->va->expr->type;
	}
 | method_designator
	{
		struct variable_access_t * variable_access = new_variable_access();
		$$ = variable_access;
	        $$->type = 4;
		$$->data.md = $1;
		$$->type = VARIABLE_ACCESS_T_METHOD_DESIGNATOR;
		$$->expr = new_expression_data();
		$$->expr->type = $1->va->expr->type;
	}
 ;

indexed_variable : variable_access LBRAC index_expression_list RBRAC
	{
		struct indexed_variable_t * indexed_variable = new_indexed_variable();
		$$ = indexed_variable;
		$$->va = $1;
		$$->iel = $3;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		if (strcasecmp($3->expr->type, PRIMITIVE_TYPE_NAME_INTEGER) != 0)
			error_array_index_is_not_integer(line_number, $1->data.id);
	}
 ;

index_expression_list : index_expression_list comma index_expression
	{
		struct index_expression_list_t * index_expression_list = new_index_expression_list();
		$$ = index_expression_list;
		$$->next = $1;
		$$->e = $3;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
	}
 | index_expression
	{
		struct index_expression_list_t * index_expression_list = new_index_expression_list();
		$$ = index_expression_list;
		$$->e = $1;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
	}
 ;

index_expression : expression ;

attribute_designator : variable_access DOT identifier
	{
		struct attribute_designator_t * attribute_designator = new_attribute_designator();
		$$ = attribute_designator;
		$$->va = $1;
		$$->id = $3;
	}
;

method_designator: variable_access DOT function_designator
	{
		struct method_designator_t * method_designator = new_method_designator();
		$$ = method_designator;
		$$->va = $1;
		$$->fd = $3;
	}
 ;

params : LPAREN actual_parameter_list RPAREN 
	{
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		$$ = actual_parameter_list;
		$$->next = $2;
	}
 ;

actual_parameter_list : actual_parameter_list comma actual_parameter
	{
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		$$ = actual_parameter_list;
		$$->next = $1;
		$$->ap = $3;
	}
 | actual_parameter 
	{
		struct actual_parameter_list_t * actual_parameter_list = new_actual_parameter_list();
		$$ = actual_parameter_list;
		$$->ap = $1;
	}
 ;

actual_parameter : expression
	{
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		$$ = actual_parameter;
		$$->e1 = $1;
	}
 | expression COLON expression
	{
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		$$ = actual_parameter;
		$$->e1 = $1;
		$$->e2 = $3;
	}
 | expression COLON expression COLON expression
	{
		struct actual_parameter_t * actual_parameter = new_actual_parameter();
		$$ = actual_parameter;
		$$->e1 = $1;
		$$->e2 = $3;
		$$->e3 = $5;
	}
 ;

boolean_expression : expression ;

expression : simple_expression
	{
		struct expression_t * expression = new_expression();
		$$ = expression;
		$$->se1 = $1;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		$$->var = $1->var;
		$$->code = $1->code;
		if (DEBUG) printf("HERE IN EXP:SE %s\n", $1->var->id);
	}
 | simple_expression relop simple_expression
	{
		struct expression_t * expression = new_expression();
		$$ = expression;
		$$->se1 = $1;
		$$->relop = $2;
		$$->se2 = $3;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		struct code_t * newcode = new_code();
		newcode->t.op_code = new_op_code();
		newcode->t.op_code->assigned = new_variable();
		newcode->t.op_code->assigned->id = nextTempId();
		newcode->t.op_code->op = $2;
		newcode->t.op_code->v1 = $1->var;
		newcode->t.op_code->v2 = $3->var;
		newcode->type = T_OP_CODE;
		$$->var = newcode->t.op_code->assigned;
		if ($1->code != NULL && $3->code == NULL)
		{
			struct code_t * tempcode = $1->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $1->code;
		}
		else if ($3->code != NULL && $1->code == NULL)
		{
			struct code_t * tempcode = $3->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $3->code;
		}
		else if ($1->code != NULL && $3->code != NULL)
		{
			struct code_t * tempcode = $1->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = $3->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $1->code;
		}
		else
		{
			$$->code = newcode;
		}
		if (DEBUG) printf("HERE IN EXP:SE %s RELOP SE %s\n", $1->var->id, $3->var->id);
	}
 ;



simple_expression : term
	{

		struct simple_expression_t * simple_expression = new_simple_expression();
		$$ = simple_expression;
		$$->t = $1;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;

		$$->var = $1->var;

		$$->code = $1->code;

		if (DEBUG) printf("HERE IN SE:TERM %s\n", $1->var->id);
	}
 | simple_expression addop term
	{
		struct simple_expression_t * simple_expression = new_simple_expression();
		$$ = simple_expression;
		$$->next = $1;
		$$->addop = $2;
		$$->t = $3;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		
		struct code_t * newcode = new_code();
		newcode->t.op_code = new_op_code();
		newcode->t.op_code->assigned = new_variable();
		newcode->t.op_code->assigned->id = nextTempId();
		newcode->t.op_code->op = $2;
		newcode->t.op_code->v1 = $1->var;
		newcode->t.op_code->v2 = $3->var;
		newcode->type = T_OP_CODE;
		$$->var = newcode->t.op_code->assigned;

		// link the lists of code together, if present
		if ($1->code != NULL && $3->code == NULL)
		{
			struct code_t * tempcode = $1->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $1->code;
		}
		else if ($3->code != NULL && $1->code == NULL)
		{
			struct code_t * tempcode = $3->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $3->code;
		}
		else if ($1->code != NULL && $3->code != NULL)
		{
			struct code_t * tempcode = $1->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = $3->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $1->code;
		}
		else
		{
			$$->code = newcode;
		}


		if (DEBUG) printf("HERE IN SE:SE %s ADDOP TERM %s = %s\n", $1->var->id, $3->var->id, $$->var->id);
	}
 ;

term : factor
	{
		struct term_t * term = new_term();
		$$ = term;
		$$->f = $1;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		$$->var = $1->var;
		if (DEBUG) printf("HERE IN TERM:FACTOR %s\n", $1->var->id);
	}
 | term mulop factor
	{
		struct term_t * term = new_term();
		$$ = term;
		$$->next = $1; 
		$$->mulop = $2;
		$$->f = $3;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		// we have regular op
		struct code_t * newcode = new_code();
		newcode->t.op_code = new_op_code();
		newcode->t.op_code->assigned = new_variable();
		newcode->t.op_code->assigned->id = nextTempId();
		newcode->t.op_code->op = $2;
		newcode->t.op_code->v1 = $1->var;
		newcode->t.op_code->v2 = $3->var;
		newcode->type = T_OP_CODE;
		$$->var = newcode->t.op_code->assigned;
		if ($1->code != NULL)
		{
			struct code_t * tempcode = $1->code;
			GOTO_END_OF_LIST(tempcode);
			tempcode->next = newcode;
			$$->code = $1->code;
		}
		else
		{
			$$->code = newcode;
		}
		if (DEBUG) printf("HERE IN TERM:TERM %s MULOP FACTOR %s\n", $1->var->id, $3->var->id);
	}
 ;

sign : PLUS
	{
        $$=yylval.i;

	}
 | MINUS
	{
        $$=yylval.i;
	}
 ;

factor : sign factor
	{
		struct factor_t * factor = new_factor();
		$$ = factor;
		$$->type = FACTOR_T_SIGNFACTOR;
		$$->data.f.sign = $1;
		$$->data.f.next = $2;
		$$->expr = new_expression_data();
		$$->expr->type = $2->expr->type;
		$$->var->id = strdup($2->var->id);
		$$->var->val.value = *($1) * $2->var->val.value;
		$$->var->type = VARIABLE_TYPE;
		if (DEBUG) printf("HERE IN FACTOR: SIGN FACTOR %s\n", $2->var->id);
	}
 | primary 
	{
		struct factor_t * factor = new_factor();
		$$ = factor;
		$$->type = FACTOR_T_PRIMARY;
		$$->data.p = $1;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		$$->var = $1->var;
		if (DEBUG) printf("HERE IN FACTOR: SIGN FACTOR %s\n", $1->var->id);
	}
 ;

primary : variable_access
	{
		struct primary_t * primary = new_primary();
		$$ = primary;
		$$->data.va = $1;
		$$->type = PRIMARY_T_VARIABLE_ACCESS;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		$$->var = $1->var;
		if (DEBUG) printf("HERE IN PRIMARY: SIGN FACTOR %s\n", $1->var->id);
	}
 | unsigned_constant
	{
		struct primary_t * primary = new_primary();
		$$ = primary;
		$$->data.un = $1;
		$$->type = PRIMARY_T_UNSIGNED_CONSTANT;
		$$->expr = new_expression_data();
		$$->expr->type = $1->expr->type;
		$$->expr->val = $1->expr->val;
		$$->var->type = CONSTANT_TYPE;
		$$->var->id = inttostring($1->ui);
		$$->var->val.value = $1->ui;
	}
 | function_designator
	{
		struct primary_t * primary = new_primary();
		$$ = primary;
		$$->data.fd = $1;
		$$->type = PRIMARY_T_FUNCTION_DESIGNATOR;
		$$->expr = new_expression_data();
	}
 | LPAREN expression RPAREN
	{
		struct primary_t * primary = new_primary();
		$$ = primary;
		$$->data.e = $2;
		$$->type = PRIMARY_T_EXPRESSION;
		$$->expr = new_expression_data();
		$$->expr->type = $2->expr->type;
	}
 | NOT primary
	{
		struct primary_t * primary = new_primary();
		$$ = primary;
		$$->type = PRIMARY_T_PRIMARY;
		$$->data.p.not = 1;
		$$->data.p.next = $2;
		$$->expr = new_expression_data();
		$$->expr->type = $2->expr->type; 
	}
 ;

unsigned_constant : unsigned_number
 ;

unsigned_number : unsigned_integer ;

unsigned_integer : DIGSEQ
	{
		struct unsigned_number_t * unsigned_number = new_unsigned_number();
		$$ = unsigned_number;
        $$->ui = *(yylval.i);
		// is this the right way to get the val from yylval
		$$->expr = new_expression_data();
		$$->expr->val = (float)(*yylval.i);
		$$->expr->type = "integer";
	}
 ;

/* functions with no params will be handled by plain identifier */
function_designator : identifier params
	{
		struct function_designator_t * function_designator = new_function_designator();
		$$ = function_designator;
 	       $$->id = $1;
        	$$->apl = $2;
		
	}
 ;


addop: PLUS
	{
		$$ = PLUS;
	}
 | MINUS
	{
		$$ = MINUS;
	}
 | OR
	{
		$$ = OR;
	}
 ;

mulop : STAR
	{
		$$ = STAR;
	}
 | SLASH
	{
		$$ = SLASH;
	}
 | MOD
	{
		$$ = MOD;
	}
 | AND
	{
		$$ = AND;
	}
 ;

relop : EQUAL
	{
		$$ = EQUAL;
	}
 | NOTEQUAL
	{
		$$ = NOTEQUAL;
	}
 | LT
	{
		$$ = LT;
	}
 | GT
	{
		$$ = GT;
	}
 | LE
	{
		$$ = LE;
	}
 | GE
	{
		$$ = GE;
	}
 ;

identifier : IDENTIFIER
	{
		$$ = strdup(yylval.id);
	}
 ;

semicolon : SEMICOLON
 ;

comma : COMMA
 ;

%%
