#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "symtab.h"

void optimize(struct program_t *p);
struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit);
void process_code(struct statement_sequence_t *code);
void build_ebb();
void find_ebb(struct basic_block_t *bb);
struct basic_block_t *ebb_wl_pop();
void ebb_wl_push(struct basic_block_t *bb);
int wl_empty();
void build_cfg();
void print_bb_list();
void print_line(struct code_t* code);
void print_bb_tree(struct basic_block_t* bb);
void print_bb(struct basic_block_t* bb);
void print_ebb_list();
void populate_value_numbers();
#endif
