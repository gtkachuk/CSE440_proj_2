#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "symtab.h"

void print_code(struct program_t *p);
void print_code_actual_parameter(struct actual_parameter_t *ap);
void print_code_actual_parameter_list(struct actual_parameter_list_t *apl);
void print_code_addop(int op);
void print_code_array_type(struct array_type_t *at);
void print_code_assignment_statement(struct assignment_statement_t *as);
void print_code_attribute_designator(struct attribute_designator_t *ad);
void print_code_boolean_expression(struct expression_t *e);
void print_code_class_block(struct class_block_t *cb);
void print_code_class_identification(struct class_identification_t *ci);
void print_code_class_list(struct class_list_t *cl);
void print_code_compound_statement(struct statement_sequence_t *ss);
void print_code_expression(struct expression_t *e);
void print_code_factor(struct factor_t *f);
void print_code_formal_parameter_list(struct formal_parameter_section_list_t *fpsl);
void print_code_formal_parameter_section(struct formal_parameter_section_t *fps);
void print_code_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl);
void print_code_func_declaration_list(struct func_declaration_list_t *fdl);
void print_code_function_block(struct function_block_t *fb);
void print_code_function_declaration(struct function_declaration_t *funcd);
void print_code_function_designator(struct function_designator_t *fdes);
void print_code_function_heading(struct function_heading_t *fh);
void print_code_function_identification(char *id);
void print_code_identifier(char *id);
void print_code_identifier_list(struct identifier_list_t *idl);
void print_code_if_statement(struct if_statement_t *is);
void print_code_index_expression(struct expression_t *e);
void print_code_index_expression_list(struct index_expression_list_t *iel);
void print_code_indexed_variable(struct indexed_variable_t *iv);
void print_code_method_designator(struct method_designator_t *md);
void print_code_mulop(int op);
void print_code_object_instantiation(struct object_instantiation_t *os);
void print_code_params(struct actual_parameter_list_t *apl);
void print_code_primary(struct primary_t *p);
void print_code_print_statement(struct print_statement_t *ps);
void print_code_range(struct range_t *r);
void print_code_relop(int op);
void print_code_result_type(char *id);
void print_code_sign(int i);
void print_code_simple_expression(struct simple_expression_t *se);
void print_code_statement(struct statement_t *s);
void print_code_statement_part(struct statement_sequence_t *ss);
void print_code_statement_sequence(struct statement_sequence_t *ss);
void print_code_term(struct term_t *t);
void print_code_type_denoter(struct type_denoter_t *tden);
void print_code_unsigned_constant(struct unsigned_number_t *un);
void print_code_unsigned_integer(struct unsigned_number_t *un);
void print_code_unsigned_number(struct unsigned_number_t *un);
void print_code_value_parameter_specification(struct formal_parameter_section_t *fps);
void print_code_variable_access(struct variable_access_t *va);
void print_code_variable_declaration(struct variable_declaration_t *vd);
void print_code_variable_declaration_list(struct variable_declaration_list_t *vdl);
void print_code_variable_declaration_func(struct variable_declaration_t *vd);
void print_code_identifier_list_func(struct identifier_list_t *il);
void print_code_variable_declaration_part(struct variable_declaration_list_t *vdl);
void print_code_variable_parameter_specification(struct formal_parameter_section_t *fps);
void print_code_while_statement(struct while_statement_t *ws);
char * opToChar(int op);