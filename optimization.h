#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "symtab.h"

void optimize(struct program_t *p);
void remove_parent(struct basic_block_t *child, struct basic_block_t *parent);
void remove_dummies(struct basic_block_t *bb);
void remove_dangling();
struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit);
void process_code(struct statement_sequence_t *code);
void attach_labels();
void build_cfg();

//print
void print_bb_list();
void print_line(struct code_t* code);
void print_bb_tree(struct basic_block_t* bb);
void print_bb(struct basic_block_t* bb);
void print_ebb_list();
void print_children(struct basic_block_t *bb);
void print_parents(struct basic_block_t *bb);
void print_program();

//value numbers 
void populate_value_numbers();

//global redundancy
int var_equal(struct variable_t *v1, struct variable_t *v2);
int get_index(struct expr_t *expr);
void find_and_replace(struct basic_block_t *bb, struct expr_t *expr, struct variable_t *tj);
void  global_redundancy_transformation();
void find_all_expressions();
void populate_de_expr(struct basic_block_t *bb);
void populate_not_expr_kill(struct basic_block_t *bb);
void populate_avail_in();

//extended block and value numbering 
void bb_value_numbering(struct basic_block_t *bb);
void ebb_value_numbering(struct basic_block_t *bb);
void extended_value_numbering();
void build_ebb();
void find_ebb(struct basic_block_t *bb);
struct basic_block_t *ebb_wl_pop();
void ebb_wl_push(struct basic_block_t *bb);
int exists_in_wl(struct basic_block_t *bb);
int wl_empty();

#endif
