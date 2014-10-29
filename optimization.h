#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "symtab.h"

void optimize(struct program_t *p);
struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit);
void process_code(struct statement_sequence_t *code);
void build_cfg();
void print_bb_list();
void print_line(struct code_t* code);
void print_bb_tree(struct basic_block_t* bb);
void print_bb(struct basic_block_t* bb);
void populate_value_numbers();
#endif
