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
  process_code(ss);
  populate_value_numbers();
  print_bb_list();
  build_cfg();

  
  temp_cl = temp_cl->next;
}

struct basic_block_t *new_bb(struct code_t *entry, struct code_t *exit)
{
  struct basic_block_t *bb = (struct basic_block_t*)malloc(sizeof(struct basic_block_t));
  bb->entry = entry;
  bb->exit = exit; 
  bb->num_incoming = 0;
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
    if(prev != NULL)
      prev->next = current;
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
        bb_idx++;
        entry = current->next;
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
        bb_idx++;
        entry = current->next;
      }

      //if the next code is label it can be target of many codes --> block cutoff unless it only has one target. need to figure out how to keep track of this.
      //else if (current->type == T_LABEL_CODE)  
      //{
      //  exit = prev;
      //  bb_list[bb_idx] = new_bb(entry,exit);
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
      process_code(ss->s->data.is->s2);
      process_code(ss->next);
      break;
    }
    if (ss->s->type == STATEMENT_T_WHILE){
      printf("Process while loop\n");
      process_code(ss->s->data.ws->s);
      process_code(ss->next);
      break;
    }
    //last statement in sequence - bb completed
    if(ss->next == NULL)
    {
      bb_list[bb_idx] = new_bb(entry, prev);
      bb_idx++;
    }
    ss = ss->next;
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


void print_bb_list()
{
	int i; 
	for (i = 0; i<bb_idx; i++)
	{
		printf("\nBASIC BLOCK %d ########################################\n", i);
		printf("Entry:\n");
		print_line(bb_list[i]->entry);
		printf("Exit:\n");
		print_line(bb_list[i]->exit);
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
							if ((val_num = value_number_for_var(bb->vt, temp_code->t.assign_code->assigned)) != NO_VALUE_NUMBER)
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
							add_constant_variable(bb->cvt,
								temp_code->t.assign_code->assigned,
								val_num);
						}
						// a = b and b does not have a value number
						else
						{
							if (DEBUG) printf("\n\nIN AC4:\n"); if (DEBUG) print_line(temp_code);
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


