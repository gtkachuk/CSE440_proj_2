#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "symtab.h"
#include "code.h"

int count = 0;

void print_code(struct program_t *p)
{
	struct class_list_t *temp_cl;
	temp_cl = p->cl;
	while (temp_cl != NULL) {

		if (temp_cl->cb->fdl != NULL)
			print_code_func_declaration_list(temp_cl->cb->fdl);

		temp_cl = temp_cl->next;
	}
}

void print_code_actual_parameter(struct actual_parameter_t *ap)
{

}

void print_code_actual_parameter_list(struct actual_parameter_list_t *apl)
{

}

void print_code_addop(int op)
{

}

void print_code_array_type(struct array_type_t *at)
{

}

void print_code_assignment_statement(struct assignment_statement_t *as)
{
	if (as == NULL)
		return;
	print_code_expression(as->e);
	if (as->code != NULL)
	{
		printf("%s = %s\n",
			as->code->t.assign_code->assigned->id,
			as->code->t.assign_code->v1->id);
	}
}

void print_code_attribute_designator(struct attribute_designator_t *ad)
{

}

void print_code_boolean_expression(struct expression_t *e)
{

}

void print_code_class_block(struct class_block_t *cb)
{

}

void print_code_class_identification(struct class_identification_t *ci)
{

}

void print_code_class_list(struct class_list_t *cl)
{

}

void print_code_compound_statement(struct statement_sequence_t *ss)
{

}

void print_code_expression(struct expression_t *e)
{
	if (e == NULL)
	{
		return;
	}
	print_code_simple_expression(e->se1);
	print_code_simple_expression(e->se2);
}

void print_code_factor(struct factor_t *f)
{
	if (DEBUG) printf("FACTOR VARIABLE %s : %d\n", f->var->id, f->var->value);
}

void print_code_formal_parameter_list(struct formal_parameter_section_list_t *fpsl)
{

}

void print_code_formal_parameter_section(struct formal_parameter_section_t *fps)
{

}

void print_code_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl)
{

}

void print_code_func_declaration_list(struct func_declaration_list_t *fdl)
{
	struct func_declaration_list_t * temp = fdl;
	while (temp != NULL)
	{
		print_code_function_declaration(temp->fd);
		temp = temp->next;
	}
}

void print_code_function_block(struct function_block_t *fb)
{
	if (fb != NULL)
		print_code_variable_declaration_part(fb->vdl);
	print_code_statement_part(fb->ss);
}



void print_code_identifier_list_func(struct identifier_list_t *il)
{

}

void print_code_function_declaration(struct function_declaration_t *funcd)
{
	if (funcd != NULL){
		print_code_function_heading(funcd->fh);
		print_code_function_block(funcd->fb);
	}
}

void print_code_function_designator(struct function_designator_t *fdes)
{

}

void print_code_function_heading(struct function_heading_t *fh)
{

}

void print_code_function_identification(char *id)
{

}

void print_code_identifier(char *id)
{

}

void print_code_identifier_list(struct identifier_list_t *idl)
{

}

void print_code_if_statement(struct if_statement_t *is)
{

}

void print_code_index_expression(struct expression_t *e)
{

}

void print_code_index_expression_list(struct index_expression_list_t *iel)
{

}

void print_code_indexed_variable(struct indexed_variable_t *iv)
{

}

void print_code_method_designator(struct method_designator_t *md)
{

}

void print_code_mulop(int op)
{

}

void print_code_object_instantiation(struct object_instantiation_t *os)
{

}

void print_code_params(struct actual_parameter_list_t *apl)
{

}

void print_code_primary(struct primary_t *p)
{

}

void print_code_print_statement(struct print_statement_t *ps)
{

}

void print_code_range(struct range_t *r)
{

}

void print_code_relop(int op)
{

}

void print_code_result_type(char *id)
{

}

void print_code_sign(int i)
{

}

void print_code_simple_expression(struct simple_expression_t *se)
{
	struct simple_expression_t * temp = se;
	while (temp != NULL)
	{
		print_code_term(temp->t);
		if (se->code != NULL)
		{
			printf("%s = %s %s %s\n",
				se->code->t.op_code->assigned->id,
				se->code->t.op_code->v1->id,
				opToChar(se->code->t.op_code->op),
				se->code->t.op_code->v2->id);
		}
		temp = temp->next;
	}

}

void print_code_statement(struct statement_t *s)
{
	struct statement_t *temp = s;
	if (temp == NULL){
		return;
	}

	switch (temp->type)
	{
		case 1: //assignment
		{
					print_code_assignment_statement(s->data.as);
					break;
		}
		case 2: //statement sequence
		{
					print_code_statement_sequence(s->data.ss);
					break;
		}
		case 3: //if
		{
					print_code_if_statement(s->data.is);
					break;
		}
		case 4: //while
		{
					print_code_while_statement(s->data.ws);
					break;
		}
		case 5: //print
		{
					print_code_print_statement(s->data.ps);
					break;
		}
	}
}

void print_code_statement_part(struct statement_sequence_t *ss)
{
	
	struct statement_sequence_t * temp = ss;

	while (temp != NULL){
		print_code_statement(temp->s);
		temp = temp->next;
	}
	
}

void print_code_statement_sequence(struct statement_sequence_t *ss)
{
	struct statement_sequence_t * temp = ss;
	while (temp != NULL)
	{
		print_code_statement(temp->s);
		temp = temp->next;
	}
}

void print_code_term(struct term_t *t)
{
	struct term_t * temp = t;
	while (temp != NULL)
	{
		print_code_factor(temp->f);
		print_code_mulop(temp->mulop);
		if (t->code != NULL)
		{
			printf("%s = %s %s %s\n",
				t->code->t.op_code->assigned->id,
				t->code->t.op_code->v1->id,
				opToChar(t->code->t.op_code->op),
				t->code->t.op_code->v2->id);
		}
		// why is the term getting printed twice..
		if (DEBUG) printf("%d\n", t);
		//int i = 1 / (count - 1);
		count++;
		temp = temp->next;
	}
}

void print_code_type_denoter(struct type_denoter_t *tden)
{

}

void print_code_unsigned_constant(struct unsigned_number_t *un)
{

}

void print_code_unsigned_integer(struct unsigned_number_t *un)
{

}

void print_code_unsigned_number(struct unsigned_number_t *un)
{

}

void print_code_value_parameter_specification(struct formal_parameter_section_t *fps)
{

}

void print_code_variable_access(struct variable_access_t *va)
{
	if (va == NULL)
		return;
	switch (va->type)
	{
		case 1:{
				   print_code_identifier(va->data.id);
				   break;
		}
		case 2:{
				   print_code_indexed_variable(va->data.iv);
				   break;
		}
		case 3:{
				   print_code_attribute_designator(va->data.ad);
				   break;
		}
		case 4:{
				   print_code_method_designator(va->data.md);
				   break;
		}
	}
}

void print_code_variable_declaration(struct variable_declaration_t *vd)
{

}

void print_code_variable_declaration_list(struct variable_declaration_list_t *vdl)
{

}

void print_code_variable_declaration_part(struct variable_declaration_list_t *vdl)
{

}

void print_code_variable_declaration_func(struct variable_declaration_t *vd)
{

}

void print_code_variable_parameter_specification(struct formal_parameter_section_t *fps)
{

}

void print_code_while_statement(struct while_statement_t *ws)
{
	// implement
}

char * opToChar(int op)
{
	switch (op)
	{
		case PLUS:
			return "+";
		case STAR:
			return "*";
		case MINUS:
			return "-";
		case SLASH:
			return "/";
		case MOD:
			return "%";
	}
	return NULL;
}
