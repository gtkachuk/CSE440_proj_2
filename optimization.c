/*
 * semantic.c
*
 * Implements all functions that participate in semantic analysis.
 */

#include "code.h"
#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "symtab.h"
#include "optimization.h"

struct  program_t *prog;
//this is temporary, can change to dynamically expand later
struct basic_block_t *bb_list[100];
int     bb_idx;
int     line_number;
extern void dprint(char * str);



/* ----------------------------------------------------------------------- 
 * Create BB and CFG structure
 * ----------------------------------------------------------------------- 
 */
void optimize(struct program_t *p)
{
  struct class_list_t *temp_cl;
  struct statement_sequence_t *ss;
  prog = p;
  temp_cl = p->cl;
  ss = temp_cl->cb->fdl->fd->fb->ss;
  bb_idx = 0;
  //if statement sequence has code
  //get first code statement, start building basic blocks
  while(ss != NULL){
    process_code(ss->s->code);
    ss = ss->next;
    
  }
  print_bb_list();
  build_cfg();

  
  temp_cl = temp_cl->next;
}

struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit) {
  struct basic_block_t *bb = (struct basic_block_t*)malloc(sizeof(struct basic_block_t));
  bb->entry = entry;
  bb->exit = exit; 
  bb->num_incoming = 0;
  return bb;
}

struct cfg_t *new_cfg(){
  struct cfg_t *cfg = (struct cfg_t *)malloc(sizeof(struct cfg_t));
  cfg->entry = NULL;
  cfg->exit = NULL;
  return cfg;
}
//-----------------------------------------------------
//recursively build all basic blocks
//-----------------------------------------------------
void process_code(struct code_t *code){
  //struct code_t *current = temp_cl->cb->fdl->fd->fb->ss;
  struct code_t *current = code;
  struct code_t *entry = code; 
  struct code_t *exit = NULL;
  struct code_t *prev = NULL;
  //continue as long as we have child code (if has true and false branches)
  while(current != NULL){
    if (current->type == T_IF_CODE)
    {
      exit = current;
      bb_list[bb_idx] = new_bb(entry,exit);
      bb_idx++;
      process_code(current->t.if_code->true_target);
      process_code(current->t.if_code->false_target);
      //here we assume that 'if' has its own true and false branch pointers, and also code->next points to the first code after the 'if' block. 
      entry = current->next;
    }
    //if the next code is label it can be target of many codes --> block cutoff unless it only has one target. need to figure out how to keep track of this.
    ///else if (current->type == T_LABEL_CODE)  
    ///{
    ///  exit = prev;
    ///  bb_list[bb_idx] = new_bb(entry,exit);
    ///  bb_idx++;
    ///}
    if (current->next == NULL) {

      bb_list[bb_idx] = new_bb(entry, current);
      bb_idx++;
    }
    prev = current;
    current = current->next;
  }
}

void build_cfg(){
  int i;
  int j;
  //find incoming edges
  for (i=0; i<bb_idx; i++)
  {
    for (j=0; j<bb_idx; j++)
    {
      if(i!=j){
        if(bb_list[i]->exit->next == bb_list[j]->entry){
          bb_list[j]->num_incoming++;      
          
        }
      }
    }
  }
  
  //merge cfgs 
  for (i=0; i<bb_idx; i++)
  {
    //create if CFGs
    if (bb_list[i]->entry->type == T_IF_CODE){
      struct cfg_t* cfg = new_cfg();
      cfg->entry = bb_list[i];
      struct code_t *dummy_code = new_code();
      cfg->entry->exit->t.if_code->true_target->next = dummy_code;
      cfg->entry->exit->t.if_code->false_target->next = dummy_code;
      dummy_code->next = cfg->entry->exit->next;
      struct basic_block_t *dummy_bb = new_bb(dummy_code, dummy_code);
      cfg->exit = dummy_bb;

    }
  }
  

}


void print_bb_list(){

  int i; 
  for (i = 0; i<bb_idx; i++)
  {
    printf("\nBASIC BLOCK %d\n", i);
    print_line(bb_list[i]->entry);
    print_line(bb_list[i]->exit);
  }
}

void print_line(struct code_t* code){
  switch(code->type)
    {
      case (T_OP_CODE):{
        printf("%s = %s op(%d) %s\n", 
                code->t.op_code->assigned->id, 
                code->t.op_code->v1->id, 
                code->t.op_code->op, 
                code->t.op_code->v2->id);        

        break;
      }
      case (T_GOTO_CODE):{
        printf("goto");
        break;
      }
      case (T_IF_CODE):{
        printf("if");
        break;
      }
      case (T_ASSIGN_CODE):{
        printf("%s = %s \n", 
        code->t.assign_code->assigned->id, 
        code->t.assign_code->v1->id); 
        break;
      }
      case (T_DUMMY):{
        printf("Dummy\n");
        break;
      }
      default:{
        printf("error: wrong code type in print\n");
      }
    }
}


