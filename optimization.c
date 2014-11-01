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
#include "symtab.h"

struct  program_t *prog;
//this is temporary, can change to dynamically expand later
#define MAX_BASIC_BLOCKS 100
struct basic_block_t *bb_list[MAX_BASIC_BLOCKS];
/*-------EBB stuff ----*/
struct basic_block_t *work_list[MAX_BASIC_BLOCKS];
struct basic_block_t *ebb_roots[MAX_BASIC_BLOCKS];
int wl_idx;
int ebb_idx;
int ebb_first;
//for extended value numbering:
struct basic_block_t *ebb_stack[MAX_BASIC_BLOCKS];
int ebb_stack_idx;
/*--------------------*/
struct cfg_t *cfg_list[MAX_BASIC_BLOCKS];
int     bb_idx;
int     line_number;
int true_exit;
int false_exit;
int     if_flag;
extern void dprint(char * str);
/*all expressions in the code*/
struct expr_t *all_expr[MAXSYMBOLS*MAXSYMBOLS];
int expr_idx;
/********** UTILITIES **************************/
//compares two variable_t's
int var_equal(struct variable_t *v1, struct variable_t *v2)
{
  int equal = 0;
  //this is wrong we need to compare types, but the types are not being assigned as I thought they were.
  //if(v1->type == v2->type)
  //{
    if(strcmp(v1->id, v2->id) == 0)
    {
      equal = 1;
    }
  //}
  return equal;
}

int expr_equal(struct expr_t *expr1, struct expr_t *expr2)
{
  int equal = 0;
  if(var_equal(expr1->v1, expr2->v1) && var_equal(expr1->v2, expr2->v2))// && (expr1->op == expr2->op))
    equal = 1;
  return equal; 
}

//remove the block from the list of blocks 
int remove_from_list(struct basic_block_t *bb)
{
  int change = 0;
  int i,j;
  for (i=0; i<bb_idx; i++)
  {
    if(bb == bb_list[i])
    {
      for(j=i+1;j<bb_idx;j++)
      {
        bb_list[j-1] = bb_list[j];
      }
      bb_idx--;
      change = 1;
    }
    
  }
  for (i=0; i<bb_idx;i++)
  {
    remove_parent(bb_list[i], bb);
  }
  return change;
}

//add block to the list of blocks parents, keep track of incoming nodes
void add_parent(struct basic_block_t *child, struct basic_block_t *parent)
{
  child->parents[child->num_incoming] = parent;
  child->num_incoming++;
}

//remove block from list of parents of block, decrease counter
void remove_parent(struct basic_block_t *child, struct basic_block_t* parent)
{
  int i;
  int j;
  for(i=0;i<child->num_incoming;i++)
  {
    if (parent == child->parents[i])
    {
      for(j=i+1;j<child->num_incoming;j++)
      {
        child->parents[j-1] = child->parents[j];
      }
      child->num_incoming--;
    }
    
  }
 if (DEBUG)  printf("ERROR COULD NOT FIND PARENT\n");
  exit_on_errors();
}

//given a code statement return the basic block of which it is the entry
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

struct basic_block_t* find_bb_by_exit(struct code_t *code)
{
  int i;

  //printf("looking for code: ");
  //print_line(code);
  for (i=0; i<bb_idx;i++)
  {
    if(bb_list[i]->exit == code){
      //printf("found BB%d\n", i);
      return bb_list[i];
    }
  }
  return NULL;
}

void stack_push(struct basic_block_t *bb)
{
  ebb_stack[ebb_stack_idx++] = bb;
}

struct basic_block_t *stack_pop()
{
  struct basic_block_t *bb;
  
  bb = ebb_stack[--ebb_stack_idx];
  ebb_stack[ebb_stack_idx] = NULL; 
  return bb;
}

struct basic_block_t *stack_peek()
{
  
  return ebb_stack[ebb_stack_idx];
  
}
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
  bb_idx = expr_idx = wl_idx = ebb_idx = ebb_first = 0;
  //if statement sequence has code
  //get first code statement, start building basic blocks
  process_code(ss);
  attach_labels();
  build_cfg();
  printf("-----------------INITIAL PRINT ----------------\n");
  remove_dangling();
  remove_dummies(bb_list[0]);
  print_main();
  if (DEBUG) printf("-----------------PRINTING BB TREE---------------\n");
  if (DEBUG) print_bb_tree(bb_list[0]);
  printf("-----------------PRINTING BB LIST---------------\n");
  print_bb_list();

  printf("---------------AFTER VALUE NUMBERING------------\n");
  populate_value_numbers();
  print_main();

  printf("---------------AFTER EXTENDED VALUE NUMBERING----\n");
  build_ebb();
  if(DEBUG) print_ebb_list();
  extended_value_numbering();
  print_main();

  if (DEBUG) print_ebb_list();
  /*gre code*/
  find_all_expressions();
  int i;
  for (i=0;i<bb_idx;i++)
  {
    populate_de_expr(bb_list[i]);
    populate_not_expr_kill(bb_list[i]);
  }
  populate_avail_in();
  global_redundancy_transformation();
  /*gre code end*/
  printf("----AFTER GLOBAL REDUNDANCY ELIMINATION-----\n");
  print_main();
  
  temp_cl = temp_cl->next;
}
/*********************
*basic block constructor
**********************/
struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit)
{
  struct basic_block_t *bb = (struct basic_block_t*)malloc(sizeof(struct basic_block_t));
  bb->entry = entry;
  bb->exit = exit; 
  bb->num_incoming = 0;
  bb->ebb_num = -1;
  bb->ebb_root = 0;
  bb->num = -1;
  bb->dummies_removed = 0;
  bb->ebb_value_numbering_done = 0;
  bb->de_expr_idx = bb->kill_idx = bb->avail_idx = 0;
  bb->avail_map = bb->de_map = 0;
  bb->vt = (struct variable_table*)malloc(sizeof(struct variable_table));
  bb->cvt = (struct variable_table*)malloc(sizeof(struct variable_table)); 
  bb->et = (struct expression_table*)malloc(sizeof(struct expression_table));
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

    //cut off previous ss, process the next ss 
    if(prev != NULL && ss->s->type == STATEMENT_T_LABEL)
    {
      prev->next = ss->s->data.l->next_ss->code;
      ss->s->data.l->prev_c = prev;
      bb_list[bb_idx] = new_bb(entry, prev);
      bb_list[bb_idx]->num = bb_idx;
      bb_idx++;
      //next statement sequence to process is the label ss
      process_code(ss->s->data.l->next_ss);
      break;
    }
    //continue chain of codes
    else if(prev != NULL) {
      prev->next = current;
      prev = NULL;
    }
    else if(prev == NULL && ss->s->type == STATEMENT_T_LABEL){
      process_code(ss->s->data.l->next_ss);
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
      else if (current->type == T_WHILE_CODE)
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
      else if (current->type == T_GOTO_CODE)
      {
        exit = current;
        bb_list[bb_idx] = new_bb(entry,exit);
        bb_list[bb_idx]->num = bb_idx;
        bb_idx++;
        //next block is not the one pointed to by goto but the next ss. This allows us to process all of the code. The children of the goto will be reassigned later. 
        entry = current->next;

      }
      

      prev = current;
      current = current->next;
    }
    //process branches as statement sequences
    //add dummies at the ends
    if (ss->s->type == STATEMENT_T_IF)
    {
      if (DEBUG) printf("Process true and false branches\n");
      process_code(ss->s->data.is->s1);
      struct basic_block_t *left = bb_list[bb_idx-1];
      process_code(ss->s->data.is->s2);
      struct basic_block_t *right = bb_list[bb_idx-1];
      struct code_t *dummy_code = new_code();
      dummy_code->type = T_DUMMY;
      struct basic_block_t *dummy_bb = new_bb(dummy_code, dummy_code);

      left->left = dummy_bb;  
      add_parent(dummy_bb, left);
      right->left = dummy_bb; 
      add_parent(dummy_bb, right);
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
      if (DEBUG) printf("Process while loop\n");
      struct code_t *dummy_code = new_code();
      struct basic_block_t *current = bb_list[bb_idx-1];
      dummy_code->type = T_DUMMY;
      struct basic_block_t *dummy_bb = new_bb(dummy_code, dummy_code);
      current->right = dummy_bb;
      add_parent(dummy_bb, current);
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
    else if (ss->s->type == STATEMENT_T_GOTO){
      if(DEBUG) printf("Process goto statement\n");
      if (ss->next == NULL)
        break;
      else{
        process_code(ss->next);
        break;
      }
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

/*make additional connections for code with labels*/
void attach_labels(){
  int i;
  struct code_t *temp;
  struct label_t *label;
  for (i=0;i<bb_idx;i++)
  {
    temp = bb_list[i]->exit;
    label = find_code_in_label(temp);
    if (label != NULL)
    {
      if (bb_list[i]->left ==NULL){
        bb_list[i]->left = find_bb_by_code(label->next_ss->code);
        add_parent(bb_list[i]->left, bb_list[i]);
      }
    }
  }

    
}
/* Merging algorithms for the blocks that haven't been merged 
during creation process. End result of this is the BB graph with dummies */
void build_cfg(){
  int i;
  for (i=0; i<bb_idx; i++)
  {
    
    if(bb_list[i]->exit->type == T_IF_CODE)
    {
      if(bb_list[i]->left == NULL){
        bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->t.if_code->true_target);
        add_parent(find_bb_by_code(bb_list[i]->exit->t.if_code->true_target), bb_list[i]);
      }
      if(bb_list[i]->right == NULL){
        bb_list[i]->right = find_bb_by_code(bb_list[i]->exit->t.if_code->false_target);
        add_parent(find_bb_by_code(bb_list[i]->exit->t.if_code->false_target), bb_list[i]);
      }
    }
    else if(bb_list[i]->exit->type == T_WHILE_CODE)
    {
      if(bb_list[i]->left == NULL){
        bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->t.if_code->true_target);
        add_parent(find_bb_by_code(bb_list[i]->exit->t.if_code->true_target), bb_list[i]);
      }
      if(bb_list[i]->left != NULL){
        bb_list[i]->left->left = bb_list[i];
        add_parent(bb_list[i], bb_list[i]->left);
      }
      //bb_list[i]->right = bb_list[i];
      //bb_list[i]->num_incoming++;
    }
    else if(bb_list[i]->exit->type == T_DUMMY)
    {
      bb_list[i]->left = find_bb_by_code(bb_list[i]->exit->next);
      add_parent(find_bb_by_code(bb_list[i]->exit->next),bb_list[i]);
    }
    else if(bb_list[i]->exit->type == T_GOTO_CODE)
    {
      //point the BB of goto to the BB under label
      bb_list[i]->left = find_bb_by_code(labelForID(bb_list[i]->exit->t.goto_code->label_id)->next_ss->s->code);
      add_parent(bb_list[i]->left, bb_list[i]);
    }
  }
}

/*get rid of dead code blocks*/
void remove_dangling()
{
  int change = 1;
  int i;
  while(change == 1){
    change = 0;
    for (i=1;i<bb_idx;i++)
    {
      if(bb_list[i]->num_incoming == 0)
      {
        if(remove_from_list(bb_list[i]) != 0)
          change = 1;
      }
    }
  }
}

/*traverse through the tree, remove dummies as in the algorithm*/
void remove_dummies(struct basic_block_t *bb){
  if(bb != NULL){
    //*node->left represents the unique child of a node
    if (bb->left != NULL && bb->left->entry->type == T_DUMMY){
      while(bb->left->entry->type == T_DUMMY && bb->left->left != NULL)
      {
        remove_parent(bb->left, bb);
        if(bb->left->num_incoming == 0)
          remove_from_list(bb->left);
        bb->left = bb->left->left;
        add_parent(bb->left, bb);
      }
    }
    if (bb->right != NULL && bb->right->entry->type == T_DUMMY){
      while(bb->right->entry->type == T_DUMMY && bb->right->left != NULL)
      {
        remove_parent(bb->right, bb);
        if(bb->right->num_incoming == 0)
          remove_from_list(bb->right);
        bb->right = bb->right->left;
        add_parent(bb->right, bb);
      }

    }
    bb->dummies_removed = 1;
    if(bb->left != NULL && bb->left->dummies_removed == 0)
      remove_dummies(bb->left);
    if(bb->right != NULL  && bb->right->dummies_removed == 0)
      remove_dummies(bb->right);
  }
}
/****************** EBB block *******************/
//Initial function, keep track of the work list
void build_ebb(){
  ebb_wl_push(bb_list[0]);
  while (!wl_empty()){
    ebb_roots[ebb_idx] = ebb_wl_pop();
    ebb_roots[ebb_idx]->ebb_num = ebb_idx;
    ebb_idx++;
    find_ebb(ebb_roots[ebb_idx-1]);
  }
}

//Find the BB's that are in this EBB and add new roots to the WL
//repeat this for every item in work list 
void find_ebb(struct basic_block_t *bb)
{
  if(bb->left != NULL && !exists_in_wl(bb->left)){
    if(bb->left->num_incoming == 1)
    {
      bb->left->ebb_num = bb->ebb_num;
      find_ebb(bb->left);
    }
    else if(bb->left->num_incoming == 0)
    {
      printf("ERROR: NO INCOMING NODES IN CHILD\n");
      exit_on_errors();
    }
    else
    {
      bb->left->ebb_root = 1;
      ebb_wl_push(bb->left);
    }

  }
  if(bb->right != NULL && !exists_in_wl(bb->right)){
    if(bb->right->num_incoming == 1)
    {
      bb->right->ebb_num = bb->ebb_num;
      find_ebb(bb->right);
    }
    else if(bb->right->num_incoming == 0)
    {
      printf("ERROR: NO INCOMING NODES IN CHILD\n");
      exit_on_errors();
    }
    else
    {
      bb->left->ebb_root = 1;
      ebb_wl_push(bb->right);
    }

  }
    
}

/*************** WORK LIST FIFO QUEUE **************/
struct basic_block_t *ebb_wl_pop()
{
  struct basic_block_t *bb = NULL;
  bb = work_list[ebb_first];
  ebb_first++;
  return bb;
  
}

void ebb_wl_push(struct basic_block_t *bb)
{
  work_list[wl_idx] = bb;
  wl_idx++;
}

int exists_in_wl(struct basic_block_t *bb)
{
  int i;
  for(i=0; i<wl_idx;i++)
  {
    if(bb == work_list[i])
      return 1;
  }
  return 0;
}

int wl_empty(){
  if (ebb_first == wl_idx)
    return 1;
  else 
    return 0;
}
/***********************WL QUEUE END***************************/


/****************************** PRINT FUNCTIONS ***********/
void print_main()
{
  label_table_print();
  print_program();
}
void print_expr(struct expr_t *expr)
{
  printf("EXPR: %s %s %s\n", expr->v1->id, opToChar(expr->op), expr->v2->id);
  
}
void print_ebb_list()
{
  int i;
  printf("---------EBB ROOTS-------\n");
  for(i=0;i<ebb_idx;i++)
  {
    print_bb(ebb_roots[i]);
  }
}

void print_bb(struct basic_block_t* bb){
  printf("#######~~~BASIC BLOCK %d ~~~########\n", bb->num);
  printf("Entry:\t");
  print_line(bb->entry);
  printf("Exit:\t");
  print_line(bb->exit);
  printf("Incoming nodes: %d\n",bb->num_incoming);
  print_children(bb);
  print_parents(bb);
  if (DEBUG) printf("GRE TABLES:\tDE: %llu KILL: %llu AVAIL: %llu\n", bb->de_map, bb->kill_map, bb->avail_map);
}

void print_children(struct basic_block_t *bb){
  printf("CHILDREN: ");
  if(bb->left != NULL)
    printf("BB_%d ", bb->left->num);
  if(bb->right != NULL)
    printf("BB_%d ", bb->right->num);
  printf("\n");
}
void print_parents(struct basic_block_t *bb){
  int i;
  printf("PARENTS: ");
  for (i=0;i<bb->num_incoming;i++)
  {
    printf("BB_%d ", bb->parents[i]->num);
  }
  printf("\n");
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
    if (DEBUG) printf("\nVARIABLE TABLE\n");
    if (DEBUG) variable_table_print(bb_list[i]->vt);
    if (DEBUG) printf("\nCONSTANT TABLE\n");
    if (DEBUG) constant_variable_table_print(bb_list[i]->cvt);
    if (DEBUG) printf("\nEXPRESSION TABLE\n");
    if (DEBUG) expression_table_print(bb_list[i]->et);
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
        printf("BRANCH %s %s %s\n", code->t.if_code->v1->id,
                                opToChar(code->t.if_code->op),
                                code->t.if_code->v2->id);

        break;
      }
      case (T_WHILE_CODE):{
        printf("BRANCH %s %s %s\n", code->t.if_code->v1->id,
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
        printf("D U M M Y\n");
        break;
      }
      default:{
        printf("error: wrong code type in print\n");
      }
    }
}

void print_gre_sets(struct basic_block_t *bb)
{
  

}

void print_program()
{
  int i;
  for(i=0;i<bb_idx;i++)
  {
    printf("~~ BASIC BLOCK %d: ~~\n", i);
    struct code_t *code = bb_list[i]->entry;
    while(code != bb_list[i]->exit->next)
    {
      print_line(code);
      code = code->next;
    }
  }
}
/************************** END PRINT FUNCTIONS ***********/

void find_all_expressions()
{
  int i;
  //for each block in the list 
  for(i=0;i<bb_idx;i++)
  {
    struct basic_block_t *bb = bb_list[i];
    struct code_t *code = bb->entry;
    //check every code in bb
    //not sure what exit->next might point to
    while(code != NULL && code != bb->exit->next)
    {
      if (DEBUG) print_line(code);
      //2 address expression can only come in an OP_CODE
      if(code->type == T_OP_CODE)
      {
        struct expr_t *expr = new_expr();
        expr->v1 = code->t.op_code->v1;
        expr->op = code->t.op_code->op;
        expr->v2 = code->t.op_code->v2;
        if(get_index(expr) == -1)
        {
          all_expr[expr_idx++] = expr;
          if (DEBUG) print_expr(expr);
        }
      }
      code = code->next;
    }
  } 
  
}

void populate_de_expr(struct basic_block_t *bb){
  struct code_t *code = bb->entry;

  //for each evaluation:
  while(code != bb->exit->next) 
  {
    //make an expression out of the code if code is of form 'a = b + c' 
    if (code->type == T_OP_CODE)
    {
      struct expr_t *expr = new_expr();
      int exposed = 1;
      expr->v1 = code->t.op_code->v1;
      expr->v2 = code->t.op_code->v2;
      //check if redefined until the end of the block
      struct code_t *temp = code->next;
      while(temp != NULL && temp != bb->exit->next)
      {
        if(temp->type == T_OP_CODE)
        {
          if (var_equal(expr->v1, temp->t.op_code->assigned)
              || var_equal(expr->v2, temp->t.op_code->assigned))
          {
            exposed = 0;
            break;
          }
        }
        else if (temp->type == T_ASSIGN_CODE)
        {
          if(var_equal(expr->v1, temp->t.assign_code->assigned)
              || var_equal(expr->v2, temp->t.assign_code->assigned))
          {
            exposed = 0;
            break;
          }
        }

        temp = temp->next; 
      }
      //if exposed add to the set of downward exposed variables
      if (exposed)
      {
        bb->de_expr[bb->de_expr_idx++] = expr;
        bb->de_map = bb->de_map | ((unsigned long long)1 << get_index(expr));
      }
    }
    code = code->next;
  }
}

//return the index of the expression in the 'all' table.
int get_index(struct expr_t *expr)
{
  int i;
  for(i=0;i<expr_idx;i++)
  {
    if(expr_equal(expr, all_expr[i]))
      return i;
  }
  return -1;
}

struct expr_t *get_expr_by_idx(int i){
  return all_expr[i];
}


void populate_not_expr_kill(struct basic_block_t *bb)
{
  struct code_t *code = bb->entry;
  bb->kill_map = (0xFFFFFFFFFFFFFFFF >> (64-expr_idx));

  int i;
  for(i=0;i<expr_idx;i++)
  {
    bb->not_expr_kill[i] = all_expr[i];
  }
  while(code != bb->exit->next)
  {
    if (code->type == T_OP_CODE)
    {
      int i;
      for (i=0;i<expr_idx;i++)
      {
        //match any of the 2 variables to kill it
        if(var_equal(code->t.op_code->assigned, all_expr[i]->v1) 
          || var_equal(code->t.op_code->assigned, all_expr[i]->v2))
        {
          //bb->not_expr_kill[bb->kill_idx++] = all_expr[i];
          bb->kill_map = bb->kill_map & ~((unsigned long long)1<< i);
        }
//do nothing
        //we are interested in expressions which are not killed in this block
        else 
        {
                  }
      }
    }
    else if (code->type == T_ASSIGN_CODE)
    {
      int i;
      for (i=0;i<expr_idx;i++)
      {
        //match any of the 2 variables to kill it
        if(var_equal(code->t.assign_code->assigned, all_expr[i]->v1) 
          || var_equal(code->t.assign_code->assigned, all_expr[i]->v2))
        {
          bb->kill_map = bb->kill_map & ~((unsigned long long)1<< i);
        } //do nothing

        //we are interested in expressions which are not killed in this block
        else 
        {
        }
      }
    }
    code = code->next;
  }
}

void populate_avail_in()
{
  int i,j,k;
  
  for(i=1;i<bb_idx;i++)
  {
    bb_list[i]->avail_map = 0xFFFFFFFFFFFFFFFF >> (64-expr_idx); //all expressions
  }
  

  int changed = 1;
  //main equations
  while(changed != 0)
  {
    changed = 0;
    for(i=0;i<bb_idx;i++)
    {
      unsigned long long map = 0xFFFFFFFFFFFFFFFF >> (64-expr_idx);
      unsigned long long old_map = bb_list[i]->avail_map;

      
      //for each predecessor
      for (j=0;j<bb_list[i]->num_incoming;j++)
      {
        map = map & (bb_list[i]->parents[j]->de_map | (bb_list[i]->parents[j]->avail_map & bb_list[i]->parents[j]->kill_map));
      }
      if (bb_list[i]->num_incoming == 0)
      {
        bb_list[i]->avail_map = 0;
      } 
      else
        bb_list[i]->avail_map = map;

      if(old_map != bb_list[i]->avail_map)
        changed = 1;


      for (k=0;k<expr_idx;k++)
      {
        if ((bb_list[i]->avail_map & (1 << k)) > 0)
        {
          bb_list[i]->avail_in[bb_list[i]->avail_idx++] = all_expr[k];
        }
      }
    }
  }
}






void  global_redundancy_transformation()
{
  //for each block
  int i,k;
  for(i=0;i<bb_idx;i++)
  {
    struct code_t *code = bb_list[i]->entry;
    while (code != bb_list[i]->exit)
    {
      //check every expression in the block. 
      if(code->type == T_OP_CODE)
      {
        struct expr_t *expr = new_expr();
        expr->v1 = code->t.op_code->v1;
        expr->v2 = code->t.op_code->v2;
        expr->op = code->t.op_code->op;
        
        //if expression is in the avail_in do the replacement algorithm
        if((bb_list[i]->avail_map & (1 << get_index(expr)))>0)
        {
          //check if the expression is killed before this assignment
          int killed = 0;
          struct code_t *temp = bb_list[i]->entry;
          while(temp != code)
          {
            if (temp->type == T_OP_CODE || temp->type == T_ASSIGN_CODE)
            {
              if (var_equal(temp->t.op_code->assigned, expr->v1) || var_equal(temp->t.op_code->assigned, expr->v2))
                killed = 1;
            }
            temp = temp->next;
          }
          if (killed)
          {
            code = code->next;
            continue;
          }
          
          //at this point it is global redundant expression
          //get a variable tj
          //search backward through all parents for the final evaluations of this expression (or we could save these positions in blocks)
          struct basic_block_t *bb = bb_list[i];

          //how do I get a new temp name
          struct variable_t *tj = new_variable();
          tj->id = nextTempId();
          //find and replace in each parent
          for(k=0;k<bb->num_incoming;k++) 
            find_and_replace(bb->parents[k], expr, tj);

          //replace code with simple assignment
          code->type = T_ASSIGN_CODE;
          //this is redundant because of how memory is distributed in structs but just in case:
          code->t.assign_code->assigned = code->t.op_code->assigned;
          code->t.assign_code->v1 = tj;

        }
      }
      code = code->next;
    }
  }
}

int is_replaced(int idx, struct expr_t *expr)
{
  int i = get_index(expr);
  return (get_expr_by_idx(i)->replaced >> idx) & 1;
}

void set_replaced(int idx, struct expr_t *expr)
{
  int i = get_index(expr);
  get_expr_by_idx(i)->replaced = get_expr_by_idx(i)->replaced | ((unsigned long long)1 << idx);
}

void find_and_replace(struct basic_block_t *bb, struct expr_t *expr, struct variable_t *tj)
{
  struct code_t *final = NULL;
  struct code_t *code = bb->entry;
  if (is_replaced(bb->num, expr))
    return;
  //find
  while(code != bb->exit->next)
  {
    if(code->type == T_OP_CODE)
    {
      struct expr_t *temp = new_expr();
      temp->v1 = code->t.op_code->v1;
      temp->v2 = code->t.op_code->v2;
      temp->op = code->t.op_code->op;
      if(expr_equal(expr, temp))
      {
        final = code;
      }
    }
    code = code->next;
  }
  if (final == NULL)
  {

  }
  else {
    //replace
    struct code_t *next = final->next;
    struct code_t *new = new_code();
    //a = tj
    new->type = T_ASSIGN_CODE;
    new->t.op_code = new_op_code();
    new->next = next;
    new->t.assign_code->assigned = final->t.op_code->assigned;
    new->t.assign_code->v1= tj;
    //tj = x + y
    final->type = T_OP_CODE;
    final->next = new;
    final->t.op_code->assigned = tj;
    final->t.op_code->v1 = expr->v1;
    final->t.op_code->v2 = expr->v2;
    set_replaced(bb->num, expr);
  }
 
  //repeat for parents 
  int i;
  for(i=0;i<bb->num_incoming;i++)
  {
    find_and_replace(bb->parents[i], expr, tj);
  }
}

void extended_value_numbering()
{
  //repeat value numbering but keeping track of the stack of basic blocks
  //copy the tables from stack to the bb being processed 
  //we're keeping track of all EBB's by roots:
  int i;
  //for each ebb root
  for(i=0;i<ebb_idx;i++)
  {
    //traverse through the ebb doing value numbering for each block
    //recursive function 
    ebb_value_numbering(ebb_roots[i]);
  }
}

void ebb_value_numbering(struct basic_block_t *bb)
{
  //do itself 
  bb_value_numbering(bb);
  bb->ebb_value_numbering_done = 1;
  //push itself onto stack
  stack_push(bb);
  //do left if its in the same ebb
  if(bb->left != NULL 
    && bb->left->ebb_num == bb->ebb_num 
    && !bb->left->ebb_value_numbering_done)
    ebb_value_numbering(bb->left);
  //do right
  if(bb->right != NULL 
    && bb->right->ebb_num == bb->ebb_num 
    && !bb->right->ebb_value_numbering_done)
    ebb_value_numbering(bb->right);
  //pop itself from stack
  stack_pop();
}

void bb_value_numbering(struct basic_block_t *bb)
{
   if(stack_peek() != NULL)
  {
    bb->vt = stack_peek()->vt;
    bb->cvt = stack_peek()->cvt;
    bb->et = stack_peek()->et;
  }
	int i;
		if (bb != NULL)
		{
			struct code_t * temp_code = bb->entry;
			while (temp_code != NULL)
			{
				int val_num = NO_VALUE_NUMBER;

				switch (temp_code->type)
				{
					case (T_OP_CODE) :
						// a = 1 + 1
						// this will also handle a = 1 * 0
						if (temp_code->t.op_code->v1->type == CONSTANT_TYPE &&
							temp_code->t.op_code->v2->type == CONSTANT_TYPE)
						{
							if (DEBUG) printf("\n\nIN OC0:\n"); if (DEBUG) print_line(temp_code);

							int val1 = temp_code->t.op_code->v1->val.constant_value;
							int val2 = temp_code->t.op_code->v2->val.constant_value;
							int new_val;
							switch (temp_code->t.op_code->op)
							{
								case PLUS:
									new_val = val1 + val2;
									break;
								case STAR:
									new_val = val1 * val2;
									break;
								case MINUS:
									new_val = val1 - val2;
									break;
								case SLASH:
									new_val = val1 / val2;
									break;
								case MOD:
									new_val = val1 % val2;
									break;
							}
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_for_constant_var(bb->cvt,
									temp_code->t.op_code->assigned,
									new_val);
							}
							else
							{
								add_constant_variable(bb->cvt,
									temp_code->t.op_code->assigned,
									new_val);
							}
							// we have an a = 2
							// so if a is in the the value_number table, remove the value_number
							if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
							}
						}

						// have one constant, check for * 0 or + 0
						else if (temp_code->t.op_code->v1->type == CONSTANT_TYPE ||
								 temp_code->t.op_code->v2->type == CONSTANT_TYPE)
						{
							int val1 = temp_code->t.op_code->v1->val.constant_value;
							int val2 = temp_code->t.op_code->v2->val.constant_value;
							int new_val;
							struct variable_t * var;
							if (val1 == NO_VALUE_NUMBER) // v1 is the variable
							{
								new_val = val2;
								var = temp_code->t.op_code->v1;
							}
							else
							{
								new_val = val1;
								var = temp_code->t.op_code->v2;
							}

							if (new_val == 0)
							{
								switch (temp_code->t.op_code->op)
								{
									case PLUS: // add 0
										if (DEBUG) printf("\n\nIN OC1:\n"); if (DEBUG) print_line(temp_code);
										if ((val_num = value_number_for_var(bb->vt, var)) == NO_VALUE_NUMBER)
										{
											val_num = add_new_variable(bb->vt, var);
										}
										// b now has a value_number, give it to a
										int assigned_val_num;
										if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											// a has an entry already, update it
											change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, val_num);
										}
										else
										{
											// a doesnt have an entry, add it
											add_variable(bb->vt,
												temp_code->t.op_code->assigned,
												val_num);
										}
										//remove the assigned from the constant table if it is there.
										if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
										{
											delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
										}
										break;
									case STAR: // multiply by 0
										if (DEBUG) printf("\n\nIN OC2:\n"); if (DEBUG) print_line(temp_code);
										if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											change_value_for_constant_var(bb->cvt,
												temp_code->t.op_code->assigned,
												0);
										}
										else
										{
											add_constant_variable(bb->cvt,
												temp_code->t.op_code->assigned,
												0);
										}
										// we have an a = 2
										// so if a is in the the value_number table, remove the value_number
										if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
										}
										break;
								}
							}
							else
							{
								if (DEBUG) printf("\n\nIN OC3:\n"); if (DEBUG) print_line(temp_code);
								if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									// a has an entry already, update it, make it different because we modified it
									change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, bb->vt->current_index++);
								}
								else
								{
									// a doesnt have an entry, add it
									add_variable(bb->vt,
										temp_code->t.op_code->assigned,
										var->val.value_number);
								}
							}
						}
						else
						{
							// no we only have 2 variables - expression fun time
							int v1 = value_number_for_var(bb->vt, temp_code->t.op_code->v1);
							if (v1 == NO_VALUE_NUMBER)
							{
								// create new value number
								if (DEBUG) printf("\n\nIN OC4: \n"); if (DEBUG) print_line(temp_code); 
								v1 = add_new_variable(bb->vt, temp_code->t.op_code->v1);
								if(DEBUG) printf("\n\nLOOKING AT : %d %d\n\n", temp_code->t.op_code->v1, temp_code->t.op_code->v1->type);
							}
							int v2 = value_number_for_var(bb->vt, temp_code->t.op_code->v2);
							if (v2 == NO_VALUE_NUMBER)
							{
								// create new value number
								if (DEBUG) printf("\n\nIN OC5:\n"); if (DEBUG) print_line(temp_code); 
								v2 = add_new_variable(bb->vt, temp_code->t.op_code->v2);
							}
							// a = b - c where b = c -> a = 0
							if (v1 == v2 && temp_code->t.op_code->op == MINUS)
							{
								if (DEBUG) printf("\n\nIN OC6:\n"); if (DEBUG) print_line(temp_code);
								if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									change_value_for_constant_var(bb->cvt,
										temp_code->t.op_code->assigned,
										0);
								}
								else
								{
									add_constant_variable(bb->cvt,
										temp_code->t.op_code->assigned,
										0);
								}
								if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
								}
							}
							else
							{
								if (DEBUG) printf("\n\nIN OC7:\n"); if (DEBUG) print_line(temp_code);
								// check if expression already exists
								struct expression_value_number_t * e = new_expression_value_number();
								e->v1 = v1;
								e->v2 = v2;
								e->op = temp_code->t.op_code->op;
								if ((val_num = value_number_for_expression(bb->et, e)) == NO_VALUE_NUMBER)
								{
									//it doesn't, create it
									val_num = add_new_expression(bb->et, e);
								}
								int assigned_val_num;
								if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									// a has an entry already, update it
									change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, val_num);
								}
								else
								{
									// a doesnt have an entry, add it
									add_variable(bb->vt,
										temp_code->t.op_code->assigned,
										val_num);
								}
								//remove the assigned from the constant table if it is there.
								if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
								{
									delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
								}
							}
						}

						break;
					case (T_GOTO_CODE) :
						break;
					case (T_IF_CODE) :
						break;
					case (T_WHILE_CODE) :
						break;
					case (T_ASSIGN_CODE) :
						// assigning a constant to a variable, add it to the constant table for now
						if (temp_code->t.assign_code->v1->type == CONSTANT_TYPE)
						{
							if (DEBUG) printf("\n\nIN AC1:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("\n\nLOOKING AT : %d\n\n", temp_code->t.assign_code->assigned);
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_for_constant_var(bb->cvt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.constant_value);
							}
							else
							{
								add_constant_variable(bb->cvt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.constant_value);
							}
							// we have an a = 1
							// so if a is in the the value_number table, remove the value_number
							if ((val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != -1)
							{
								delete_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned);
							}
						}
						// we have an a = b situation
						// b has a value_number, give it to a
						else if ((val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->v1)) != NO_VALUE_NUMBER)
						{
							int assigned_val_num;
							if (DEBUG) printf("\n\nIN AC2:\n"); if (DEBUG) print_line(temp_code);
							if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned, val_num);
							}
							else
							{
								add_variable(bb->vt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.value_number);
							}
							// remove a from the constant table if applicable
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
							}
						}
						// for a = b and b is a constant, make a a constant with the same value
						else if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->v1)) != NO_VALUE_NUMBER)
						{
							if (DEBUG) printf("\n\nIN AC3:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("IN VN3: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
							add_constant_variable(bb->cvt,
								temp_code->t.assign_code->assigned,
								val_num);
						}
						// a = b and b does not have a value number
						else
						{
							if (DEBUG) printf("\n\nIN AC4:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("IN VN4: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
							add_new_variable(bb->vt,
								temp_code->t.assign_code->v1);
							// b now has a value_number, give it to a
							int assigned_val_num;
							if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								// a has an entry already, update it
								change_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned, val_num);
							}
							else
							{
								// a doesnt have an entry, add it
								add_variable(bb->vt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.value_number);
							}
							//remove the assigned from the constant table if it is there.
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
							}
						}
						break;
				}
				temp_code = temp_code->next;
			}
		}
}


// populate the value_number table for the basic block as well as the constant_value_number_table
// will perform constant folding at the same time
void populate_value_numbers()
{
	int i;


	for (i = 0; i < MAX_BASIC_BLOCKS; i++)
	{
		if (bb_list[i] != NULL)
		{
			struct basic_block_t * bb = bb_list[i];
			struct code_t * temp_code = bb->entry;
			while (temp_code != NULL)
			{
				int val_num = NO_VALUE_NUMBER;

				switch (temp_code->type)
				{
					case (T_OP_CODE) :
						// a = 1 + 1
						// this will also handle a = 1 * 0
						if (temp_code->t.op_code->v1->type == CONSTANT_TYPE &&
							temp_code->t.op_code->v2->type == CONSTANT_TYPE)
						{
							if (DEBUG) printf("\n\nIN OC0:\n"); if (DEBUG) print_line(temp_code);

							int val1 = temp_code->t.op_code->v1->val.constant_value;
							int val2 = temp_code->t.op_code->v2->val.constant_value;
							int new_val;
							switch (temp_code->t.op_code->op)
							{
								case PLUS:
									new_val = val1 + val2;
									break;
								case STAR:
									new_val = val1 * val2;
									break;
								case MINUS:
									new_val = val1 - val2;
									break;
								case SLASH:
									new_val = val1 / val2;
									break;
								case MOD:
									new_val = val1 % val2;
									break;
							}
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_for_constant_var(bb->cvt,
									temp_code->t.op_code->assigned,
									new_val);
							}
							else
							{
								add_constant_variable(bb->cvt,
									temp_code->t.op_code->assigned,
									new_val);
							}
							// we have an a = 2
							// so if a is in the the value_number table, remove the value_number
							if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
							}
						}

						// have one constant, check for * 0 or + 0
						else if (temp_code->t.op_code->v1->type == CONSTANT_TYPE ||
								 temp_code->t.op_code->v2->type == CONSTANT_TYPE)
						{
							int val1 = temp_code->t.op_code->v1->val.constant_value;
							int val2 = temp_code->t.op_code->v2->val.constant_value;
							int new_val;
							struct variable_t * var;
							if (val1 == NO_VALUE_NUMBER) // v1 is the variable
							{
								new_val = val2;
								var = temp_code->t.op_code->v1;
							}
							else
							{
								new_val = val1;
								var = temp_code->t.op_code->v2;
							}

							if (new_val == 0)
							{
								switch (temp_code->t.op_code->op)
								{
									case PLUS: // add 0
										if (DEBUG) printf("\n\nIN OC1:\n"); if (DEBUG) print_line(temp_code);
										if ((val_num = value_number_for_var(bb->vt, var)) == NO_VALUE_NUMBER)
										{
											val_num = add_new_variable(bb->vt, var);
										}
										// b now has a value_number, give it to a
										int assigned_val_num;
										if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											// a has an entry already, update it
											change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, val_num);
										}
										else
										{
											// a doesnt have an entry, add it
											add_variable(bb->vt,
												temp_code->t.op_code->assigned,
												val_num);
										}
										//remove the assigned from the constant table if it is there.
										if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
										{
											delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
										}
										break;
									case STAR: // multiply by 0
										if (DEBUG) printf("\n\nIN OC2:\n"); if (DEBUG) print_line(temp_code);
										if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											change_value_for_constant_var(bb->cvt,
												temp_code->t.op_code->assigned,
												0);
										}
										else
										{
											add_constant_variable(bb->cvt,
												temp_code->t.op_code->assigned,
												0);
										}
										// we have an a = 2
										// so if a is in the the value_number table, remove the value_number
										if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
										{
											delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
										}
										break;
								}
							}
							else
							{
								if (DEBUG) printf("\n\nIN OC3:\n"); if (DEBUG) print_line(temp_code);
								if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									// a has an entry already, update it, make it different because we modified it
									change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, bb->vt->current_index++);
								}
								else
								{
									// a doesnt have an entry, add it
									add_variable(bb->vt,
										temp_code->t.op_code->assigned,
										var->val.value_number);
								}
							}
						}
						else
						{
							// no we only have 2 variables - expression fun time
							int v1 = value_number_for_var(bb->vt, temp_code->t.op_code->v1);
							if (v1 == NO_VALUE_NUMBER)
							{
								// create new value number
								if (DEBUG) printf("\n\nIN OC4: \n"); if (DEBUG) print_line(temp_code); 
								v1 = add_new_variable(bb->vt, temp_code->t.op_code->v1);
								if (DEBUG) printf("\n\nLOOKING AT : %d %d\n\n", temp_code->t.op_code->v1, temp_code->t.op_code->v1->type);
							}
							int v2 = value_number_for_var(bb->vt, temp_code->t.op_code->v2);
							if (v2 == NO_VALUE_NUMBER)
							{
								// create new value number
								if (DEBUG) printf("\n\nIN OC5:\n"); if (DEBUG) print_line(temp_code); 
								v2 = add_new_variable(bb->vt, temp_code->t.op_code->v2);
							}
							// a = b - c where b = c -> a = 0
							if (v1 == v2 && temp_code->t.op_code->op == MINUS)
							{
								if (DEBUG) printf("\n\nIN OC6:\n"); if (DEBUG) print_line(temp_code);
								if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									change_value_for_constant_var(bb->cvt,
										temp_code->t.op_code->assigned,
										0);
								}
								else
								{
									add_constant_variable(bb->cvt,
										temp_code->t.op_code->assigned,
										0);
								}
								if ((val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									delete_value_number_for_var(bb->vt, temp_code->t.op_code->assigned);
								}
							}
							else
							{
								if (DEBUG) printf("\n\nIN OC7:\n"); if (DEBUG) print_line(temp_code);
								// check if expression already exists
								struct expression_value_number_t * e = new_expression_value_number();
								e->v1 = v1;
								e->v2 = v2;
								e->op = temp_code->t.op_code->op;
								if ((val_num = value_number_for_expression(bb->et, e)) == NO_VALUE_NUMBER)
								{
									//it doesn't, create it
									val_num = add_new_expression(bb->et, e);
								}
								int assigned_val_num;
								if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.op_code->assigned)) != NO_VALUE_NUMBER)
								{
									// a has an entry already, update it
									change_value_number_for_var(bb->vt, temp_code->t.op_code->assigned, val_num);
								}
								else
								{
									// a doesnt have an entry, add it
									add_variable(bb->vt,
										temp_code->t.op_code->assigned,
										val_num);
								}
								//remove the assigned from the constant table if it is there.
								if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
								{
									delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
								}
							}
						}

						break;
					case (T_GOTO_CODE) :
						break;
					case (T_IF_CODE) :
						break;
					case (T_WHILE_CODE) :
						break;
					case (T_ASSIGN_CODE) :
						// assigning a constant to a variable, add it to the constant table for now
						if (temp_code->t.assign_code->v1->type == CONSTANT_TYPE)
						{
							if (DEBUG) printf("\n\nIN AC1:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("\n\nLOOKING AT : %d\n\n", temp_code->t.assign_code->assigned);
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_for_constant_var(bb->cvt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.constant_value);
							}
							else
							{
								add_constant_variable(bb->cvt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.constant_value);
							}
							// we have an a = 1
							// so if a is in the the value_number table, remove the value_number
							if ((val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != -1)
							{
								delete_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned);
							}
						}
						// we have an a = b situation
						// b has a value_number, give it to a
						else if ((val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->v1)) != NO_VALUE_NUMBER)
						{
							int assigned_val_num;
							if (DEBUG) printf("\n\nIN AC2:\n"); if (DEBUG) print_line(temp_code);
							if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								change_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned, val_num);
							}
							else
							{
								add_variable(bb->vt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.value_number);
							}
							// remove a from the constant table if applicable
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
							}
						}
						// for a = b and b is a constant, make a a constant with the same value
						else if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->v1)) != NO_VALUE_NUMBER)
						{
							if (DEBUG) printf("\n\nIN AC3:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("IN VN3: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
							add_constant_variable(bb->cvt,
								temp_code->t.assign_code->assigned,
								val_num);
						}
						// a = b and b does not have a value number
						else
						{
							if (DEBUG) printf("\n\nIN AC4:\n"); if (DEBUG) print_line(temp_code);
							if (DEBUG) printf("IN VN4: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
							add_new_variable(bb->vt,
								temp_code->t.assign_code->v1);
							// b now has a value_number, give it to a
							int assigned_val_num;
							if ((assigned_val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								// a has an entry already, update it
								change_value_number_for_var(bb->vt, temp_code->t.assign_code->assigned, val_num);
							}
							else
							{
								// a doesnt have an entry, add it
								add_variable(bb->vt,
									temp_code->t.assign_code->assigned,
									temp_code->t.assign_code->v1->val.value_number);
							}
							//remove the assigned from the constant table if it is there.
							if ((val_num = value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
							{
								delete_value_for_constant_var(bb->cvt, temp_code->t.assign_code->assigned);
							}
						}
						break;
				}
				temp_code = temp_code->next;
			}
		}
	}
}


