#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
void optimize(struct program_t *p);
struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit);
void process_code(struct code_t *code);
void build_cfg();
void print_bb_list();
void print_line(struct code_t* code);

#endif
