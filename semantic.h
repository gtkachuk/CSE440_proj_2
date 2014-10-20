/*
 * semantic.h
 *
 */

#ifndef SEMANTIC_H
#define SEMANTIC_H

void exit_on_errors();
struct class_list_t * lookup_class_by_id(char * id);
void semantic_analysis(struct program_t *p);
void semantic_check_actual_parameter(struct actual_parameter_t *ap);
void semantic_check_actual_parameter_list(struct actual_parameter_list_t *apl);
void semantic_check_addop(int *op);
void semantic_check_array_type(struct array_type_t *at);
void semantic_check_assignment_statement(struct assignment_statement_t *as);
void semantic_check_attribute_designator(struct attribute_designator_t *ad);
void semantic_check_boolean_expression(struct expression_t *e);
void semantic_check_class_block(struct class_block_t *cb);
void semantic_check_class_identification(struct class_identification_t *ci);
void semantic_check_class_list(struct class_list_t *cl);
void semantic_check_compound_statement(struct statement_sequence_t *ss);
void semantic_check_expression(struct expression_t *e);
void semantic_check_factor(struct factor_t *f);
void semantic_check_formal_parameter_list(struct formal_parameter_section_list_t *fpsl);
void semantic_check_formal_parameter_section(struct formal_parameter_section_t *fps);
void semantic_check_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl);
void semantic_check_func_declaration_list(struct func_declaration_list_t *fdl);
void semantic_check_function_block(struct function_block_t *fb);
void semantic_check_function_declaration(struct function_declaration_t *funcd);
void semantic_check_function_designator(struct function_designator_t *fdes);
void semantic_check_function_heading(struct function_heading_t *fh);
void semantic_check_function_identification(char *id);
void semantic_check_identifier(char *id);
void semantic_check_identifier_list(struct identifier_list_t *idl);
void semantic_check_if_statement(struct if_statement_t *is);
void semantic_check_index_expression(struct expression_t *e);
void semantic_check_index_expression_list(struct index_expression_list_t *iel);
void semantic_check_indexed_variable(struct indexed_variable_t *iv);
void semantic_check_method_designator(struct method_designator_t *md);
void semantic_check_mulop(int *op);
void semantic_check_object_instantiation(struct object_instantiation_t *os);
void semantic_check_params(struct actual_parameter_list_t *apl);
void semantic_check_primary(struct primary_t *p);
void semantic_check_print_statement(struct print_statement_t *ps);
void semantic_check_range(struct range_t *r);
void semantic_check_relop(int *op);
void semantic_check_result_type(char *id);
void semantic_check_sign(int *i);
void semantic_check_simple_expression(struct simple_expression_t *se);
void semantic_check_statement(struct statement_t *s);
void semantic_check_statement_part(struct statement_sequence_t *ss);
void semantic_check_statement_sequence(struct statement_sequence_t *ss);
void semantic_check_term(struct term_t *t);
void semantic_check_type_denoter(struct type_denoter_t *tden);
void semantic_check_unsigned_constant(struct unsigned_number_t *un);
void semantic_check_unsigned_integer(struct unsigned_number_t *un);
void semantic_check_unsigned_number(struct unsigned_number_t *un);
void semantic_check_value_parameter_specification(struct formal_parameter_section_t *fps);
void semantic_check_variable_access(struct variable_access_t *va);
void semantic_check_variable_declaration(struct variable_declaration_t *vd);
void semantic_check_variable_declaration_list(struct variable_declaration_list_t *vdl);
  void semantic_check_variable_declaration_func(struct variable_declaration_t *vd);
  void semantic_check_identifier_list_func(struct identifier_list_t *il);
void semantic_check_variable_declaration_part(struct variable_declaration_list_t *vdl);
void semantic_check_variable_parameter_specification(struct formal_parameter_section_t *fps);
void semantic_check_while_statement(struct while_statement_t *ws);

#endif /* SEMANTIC_H */
