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
/*--------------------*/
struct cfg_t *cfg_list[MAX_BASIC_BLOCKS];
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
  populate_value_numbers();
  printf("-----------------PRINTING BB LIST---------------\n");
  print_bb_list();
  build_cfg();
  printf("-----------------PRINTING BB TREE---------------\n");
  print_bb_tree(bb_list[0]);
  build_ebb();
  print_ebb_list();

  
  temp_cl = temp_cl->next;
}

struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit)
{
  struct basic_block_t *bb = (struct basic_block_t*)malloc(sizeof(struct basic_block_t));
  bb->entry = entry;
  bb->exit = exit; 
  bb->num_incoming = 0;
  bb->ebb_num = -1;
  bb->ebb_root = 0;
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

void build_ebb(){
  ebb_wl_push(bb_list[0]);
  while (!wl_empty()){
    ebb_roots[ebb_idx] = ebb_wl_pop();
    ebb_roots[ebb_idx]->ebb_num = ebb_idx;
    ebb_idx++;
    find_ebb(ebb_roots[ebb_idx-1]);
  }
}

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

void print_ebb_list()
{
  int i;
  printf("---------EBB ROOTS-------\n");
  for(i=0;i<ebb_idx;i++)
  {
    print_bb(ebb_roots[i]);
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
        bb_list[i]->num_incoming++;
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
  /*  
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
  printf("#######~~~BASIC BLOCK %d ~~~########\n", bb->num);
  printf("Entry:\n");
  print_line(bb->entry);
  printf("Exit:\n");
  print_line(bb->exit);
  printf("Is root: %d\n", bb->ebb_root);
  printf("Incoming nodes: %d\n",bb->num_incoming);
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
    printf("\nVARIABLE TABLE\n");
    variable_table_print(bb_list[i]->vt);
    printf("\nCONSTANT TABLE\n");
    constant_variable_table_print(bb_list[i]->cvt);
    printf("\nEXPRESSION TABLE\n");
    expression_table_print(bb_list[i]->et);
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
								if (DEBUG) printf("\n\nIN OC4:\n"); if (DEBUG) print_line(temp_code); 
								v1 = add_new_variable(bb->vt, temp_code->t.op_code->v1);
								printf("\n\nLOOKING AT : %d\n\n", temp_code->t.op_code->v1);
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
							printf("\n\nLOOKING AT : %d\n\n", temp_code->t.assign_code->assigned);
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
							printf("IN VN3: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
							add_constant_variable(bb->cvt,
								temp_code->t.assign_code->assigned,
								val_num);
						}
						// a = b and b does not have a value number
						else
						{
							if (DEBUG) printf("\n\nIN AC4:\n"); if (DEBUG) print_line(temp_code);
							printf("IN VN4: %s %d\n", temp_code->t.assign_code->v1->id, temp_code->t.assign_code->v1->val.constant_value); 
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


