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
struct cfg_t *cfg_list[100];
int     bb_idx;
int     line_number;
int true_exit;
int false_exit;
int     if_flag;
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
  process_code(ss);
  printf("-----------------PRINTING BB LIST---------------\n");
  print_bb_list();
  build_cfg();
  printf("-----------------PRINTING BB TREE---------------\n");
  print_bb_tree(bb_list[0]);

  
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
void process_code(struct statement_sequence_t *ss){
  struct code_t *current = ss->code;
  struct code_t *entry = current; 
  struct code_t *exit = NULL;
  struct code_t *prev = NULL;
  //for each statement
  while(ss!= NULL){
    
    current = ss->code;

    //continue chain of codes
    if(prev != NULL) {
      prev->next = current;
      prev = NULL;
    }
    //for each code in this statement
    while(current != NULL){
      print_line(current);
      //basic block end with if statement
      if (current->type == T_IF_CODE)
      {
        if (ss->s->type != STATEMENT_T_IF)
        {
          printf("CODE IS IF BUT STATEMENT IS NOT IF\n");
          exit_on_errors();
        }
        exit = current;

        bb_list[bb_idx] = new_bb(entry,exit);
        bb_list[bb_idx]->num = bb_idx;
        bb_idx++;
        entry = current->next;
        break;
      }
      if (current->type == T_WHILE_CODE)
      {
        if (ss->s->type != STATEMENT_T_WHILE)
        {
          printf("CODE IS WHILE BUT STATEMENT IS NOT WHILE\n");
          exit_on_errors();
        }
        exit = current;
        bb_list[bb_idx] = new_bb(entry,exit);
        bb_list[bb_idx]->num = bb_idx;
        bb_idx++;
        entry = current->next;
        break;
      }
      

      //if the next code is label it can be target of many codes --> block cutoff unless it only has one target. need to figure out how to keep track of this.
      //else if (current->type == T_LABEL_CODE)  
      //{
      //  exit = prev;
      //  bb_list[bb_idx] = new_bb(entry,exit);
        //bb_list[bb_idx]->num = bb_idx;
      //  bb_idx++;
      //}
      prev = current;
      current = current->next;
    }
    //process branches as statement sequences
    if (ss->s->type == STATEMENT_T_IF)
    {
      printf("Process true and false branches\n");
      process_code(ss->s->data.is->s1);
      struct basic_block_t *left = bb_list[bb_idx-1];
      process_code(ss->s->data.is->s2);
      struct basic_block_t *right = bb_list[bb_idx-1];
      struct code_t *dummy_code = new_code();
      dummy_code->type = T_DUMMY;
      struct basic_block_t *dummy_bb = new_bb(dummy_code, dummy_code);
      left->left = dummy_bb;  dummy_bb->num_incoming++;
      right->left = dummy_bb; dummy_bb->num_incoming++;
      if (ss->next != NULL)
      {
        dummy_bb->exit->next = ss->next->code;
        entry = ss->next->code;
      }
      bb_list[bb_idx] = dummy_bb;
      bb_list[bb_idx]->num = bb_idx;
      bb_idx++;
      //process_code(ss->next);
    }
    else if (ss->s->type == STATEMENT_T_WHILE){
      printf("Process while loop\n");
      struct code_t *dummy_code = new_code();
      struct basic_block_t *current = bb_list[bb_idx-1];
      dummy_code->type = T_DUMMY;
      struct basic_block_t *dummy_bb = new_bb(dummy_code, dummy_code);
      current->right = dummy_bb;
      dummy_bb->num_incoming++;
      bb_list[bb_idx] = dummy_bb;
      bb_list[bb_idx]->num = bb_idx;
      bb_idx++;
      process_code(ss->s->data.ws->s);
      if (ss->next != NULL)
      {
        dummy_bb->exit->next = ss->next->code;
        entry = ss->next->code;
      }
      

      //process_code(ss->next);
    }
    //last statement in sequence - bb completed
    if(ss->next == NULL)
    {
      bb_list[bb_idx] = new_bb(entry, prev);
      bb_list[bb_idx]->num = bb_idx;
      bb_idx++;
    }
    ss = ss->next;
  }
}

struct basic_block_t* find_bb_by_code(struct code_t *code)
{
  int i;

  //printf("looking for code: ");
  //print_line(code);
  for (i=0; i<bb_idx;i++)
  {
    if(bb_list[i]->entry == code){
      //printf("found BB%d\n", i);
      return bb_list[i];
    }
  }
  return NULL;
}

void build_cfg(){
  int i;
  int j;
  for (i=0; i<bb_idx; i++)
  {
    if(bb_list[i]->exit->type == T_IF_CODE)
    {
      if(bb_list[i]->left == NULL){
        bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->t.if_code->true_target);
        find_bb_by_code(bb_list[i]->exit->t.if_code->true_target)->num_incoming++;
      }
      if(bb_list[i]->right == NULL){
        bb_list[i]->right = find_bb_by_code(bb_list[i]->exit->t.if_code->false_target);
        find_bb_by_code(bb_list[i]->exit->t.if_code->false_target)->num_incoming++;
      }
    }
    else if(bb_list[i]->exit->type == T_WHILE_CODE)
    {
      if(bb_list[i]->left == NULL){
        bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->t.if_code->true_target);
        find_bb_by_code(bb_list[i]->exit->t.if_code->true_target)->num_incoming++;
      }
      if(bb_list[i]->left != NULL){
        bb_list[i]->left->left = bb_list[i];
      }
      //bb_list[i]->right = bb_list[i];
      //bb_list[i]->num_incoming++;
    }
    else if(bb_list[i]->exit->type == T_DUMMY)
    {
      bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->next);
      bb_list[i]->left->num_incoming++;
    }
  }
  //find incoming edges
  /*
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
  */
  

}


void print_bb(struct basic_block_t* bb){
  printf("BASIC BLOCK %d\n", bb->num);
  printf("Entry:\n");
  print_line(bb->entry);
  printf("Exit:\n");
  print_line(bb->exit);
}

void print_bb_tree(struct basic_block_t* bb){
  if(bb != NULL && bb->printed == 0)
  {
    bb->printed = 1;
    print_bb(bb);
    printf("\n");
    printf("LEFT: \n");
    print_bb_tree(bb->left);
    printf("\n");
    printf("RIGHT: \n");
    print_bb_tree(bb->right);
    printf("\n");
  }
  else if (bb != NULL && bb->printed == 1)
    printf("BB_%d\n", bb->num);

}

void print_bb_list(){

  int i; 
  for (i = 0; i<bb_idx; i++)
  {
    print_bb(bb_list[i]);
  }
}

void print_line(struct code_t* code){
  switch(code->type)
    {
      case (T_OP_CODE):{
        printf("%s = %s %s %s\n", 
                code->t.op_code->assigned->id, 
                code->t.op_code->v1->id, 
                opToChar(code->t.op_code->op), 
                code->t.op_code->v2->id);        

        break;
      }
      case (T_GOTO_CODE):{
        printf("goto\n");
        break;
      }
      case (T_IF_CODE):{
        printf("if %s %s %s\n", code->t.if_code->v1->id,
                                opToChar(code->t.if_code->op),
                                code->t.if_code->v2->id);

        break;
      }
      case (T_WHILE_CODE):{
        printf("while %s %s %s\n", code->t.if_code->v1->id,
                                opToChar(code->t.if_code->op),
                                code->t.if_code->v2->id);
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


