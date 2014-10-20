/*
 * semantic.c
*
 * Implements all functions that participate in semantic analysis.
 */


#include "shared.h"
#include "semantic.h"
#include "rulefuncs.h"
#include "usrdef.h"
#include "symtab.h"

struct  program_t *prog;
struct  class_list_t  *cur_class_scope;
struct  func_declaration_list_t *cur_function_scope;
int     line_number;
extern void dprint(char * str);

int   lookup_var(char * id)
{
  
    //Has this variable been declared?
    struct class_list_t *cl = cur_class_scope;
    struct variable_declaration_list_t *vdl = NULL;
    struct identifier_list_t *il = NULL;
    struct func_declaration_list_t *fl = cur_function_scope;
    int declared = 0;
    
    //in class/ancestor's scope?
    while (cl != NULL)
    {
      
      if (cl->cb == NULL)
        error_compiler_error(line_number, "No block in class");
      vdl = cl->cb->vdl; 
      while (vdl != NULL)
      {
        if (vdl->vd == NULL)
          break;
        il = vdl->vd->il;
        while (il != NULL)
        {
          if (il->id == NULL)
            error_compiler_error(line_number, "ID LIST HAS NO ID");
          if (strcmp(il->id, id) == 0)
            declared = 1;
          il = il->next;  
        }
        vdl = vdl->next;
      }
     
      if(cl->ci == NULL)
        error_compiler_error(line_number, "CLASS HAS NO ID");
      if(cl->ci->extend != NULL)
        cl=lookup_class_by_id(cl->ci->extend);
      else
        cl=NULL;
    }

    vdl = NULL;
    il = NULL;
    fl = cur_function_scope;
     
    //in function local declarations?
    if (fl != NULL)
    {
      if (fl->fd == NULL)
        error_compiler_error(line_number, "FUNCTION HAS NO DECLARATION");
      if (fl->fd->fb != NULL)
      {
        vdl = fl->fd->fb->vdl;
        while (vdl != NULL)
        {
          if (vdl->vd == NULL || vdl->vd->il == NULL)
            break;
          il = vdl->vd->il;
          while (il != NULL)
          {
            if (il->id == NULL)
              break;
            if (strcmp(il->id, id) == 0)
              declared = 1;
            il = il->next;  
          }
          
          vdl = vdl->next;
        }
      }
    }

    //in function parameters?
    struct formal_parameter_section_list_t *fpsl = fl->fd->fh->fpsl;
    while (fpsl != NULL)
    {

      if (fpsl->fps == NULL)
        break;
      il = fpsl->fps->il;
      while (il != NULL)
      {
        if(strcmp(il->id, id) == 0)
          declared = 1;

        il = il->next;
      }
      fpsl = fpsl->next;
    }
    return declared;
}

struct  class_list_t * lookup_class_by_id(char * id){
  struct class_list_t * temp;
  
  temp = prog->cl;

  
  while(temp != NULL)
  {
      #if DEBUG
      printf("\n%s", temp->ci->id);
      #endif
      if(temp->ci != NULL && strcmp(temp->ci->id, id) == 0){

        return temp;
      }
      temp = temp->next;
  }
  return NULL;
}

/* ----------------------------------------------------------------------- 
 * Carries out semantic analysis on a program
 * ----------------------------------------------------------------------- 
 */
void semantic_analysis(struct program_t *p)
{
  struct class_list_t *temp_cl;
  int main_class_exists = 0;
  prog = p;
  temp_cl = p->cl;
  while (temp_cl != NULL) {
    cur_class_scope = temp_cl;
     
    if (strcmp(temp_cl->ci->id, p->ph->id) == 0){
        dprint("SEM_ANALYSIS::: Main class exists");
        main_class_exists = 1;
    }

    if(temp_cl->ci != NULL)
        semantic_check_class_identification(temp_cl->ci);

    /* process the variable_declaration_list */
    
    if(temp_cl->cb->vdl != NULL)
      semantic_check_variable_declaration_list(temp_cl->cb->vdl);


    /* process the func_declaration_list */
    if(temp_cl->cb->fdl != NULL)
      semantic_check_func_declaration_list(temp_cl->cb->fdl);

    temp_cl = temp_cl->next;
  }


  //if there is no main class:
  if (main_class_exists == 0){
      error_missing_program_class();
  }

}

  void semantic_check_actual_parameter(struct actual_parameter_t *ap)
  {

  }

  void semantic_check_actual_parameter_list(struct actual_parameter_list_t *apl)
  {

  }

  void semantic_check_addop(int *op)
  {

  }

  void semantic_check_array_type(struct array_type_t *at)
  {

  }

  void semantic_check_assignment_statement(struct assignment_statement_t *as)
  {
    if (as == NULL)
      return;
    semantic_check_variable_access(as->va);
    semantic_check_expression(as->e);
    //semantic_check_object_instantiation(as->oe);

  }

  void semantic_check_attribute_designator(struct attribute_designator_t *ad)
  {
    struct class_list_t * cl = cur_class_scope;
    struct variable_declaration_list_t *vdl;
    struct identifier_list_t *il;
    int declared = 0;
    //its this look for it in class and parents 
    if (ad->va->type == 1 && strcmp(ad->id, "this") == 0)
    {
      while (cl != NULL)
      {
        
        if (cl->cb == NULL)
          error_compiler_error(line_number, "No block in class");
        vdl = cl->cb->vdl; 
        while (vdl != NULL)
        {
          if (vdl->vd == NULL)
            break;
          il = vdl->vd->il;
          while (il != NULL)
          {
            if (il->id == NULL)
              error_compiler_error(line_number, "ID LIST HAS NO ID");
            if (strcmp(il->id, ad->id) == 0)
              declared = 1;
            il = il->next;  
          }
          vdl = vdl->next;
        }
       
        if (cl->ci != NULL && cl->ci->extend != NULL)
          cl = lookup_class_by_id(cl->ci->extend); 
        else
          cl = NULL;

      }
    }
    /* 
    if (declared = 0)
      error_variable_not_declared(line_number, ad->id);
    else 
      error_compiler_error(line_number, "THIS.\n");
     */ 


  }

  void semantic_check_boolean_expression(struct expression_t *e)
  {

  }

  void semantic_check_class_block(struct class_block_t *cb)
  {

  }

  void semantic_check_class_identification(struct class_identification_t *ci)
  {
    struct class_list_t  * temp = prog->cl;
    
    while (temp != NULL)
    {
      if (temp->ci == NULL || temp->ci->id == NULL || ci->id == NULL)
        break;
      if (temp->ci != ci && strcmp(temp->ci->id, ci->id) == 0) {
        error_class_already_declared(ci->line_number, ci->id, temp->ci->line_number);
        exit_on_errors();
      }
      temp = temp->next;
    }


    dprint("SEMANTICS:::  class id check");
    //id exists tested in symtable but doesnt work.
    
    
    //what is _USER type for?
    //see if extend class exists
    //how do I get global scope?
    if(ci->extend != NULL){
        dprint("LOOKUP CLASS::: LOOKING UP CLASS");
        if(lookup_class_by_id(ci->extend)==NULL) {
            dprint("LOOKUP CLASS::: NO SUCH CLASS");
            error_extending_missing_class(ci->line_number, ci->id, ci->extend); 
        }
    }
     
        
  }

  void semantic_check_class_list(struct class_list_t *cl)
  {

  }

  void semantic_check_compound_statement(struct statement_sequence_t *ss)
  {

  }

  void semantic_check_expression(struct expression_t *e)
  {
    if (e == NULL)
    {
      return;
    }
    semantic_check_simple_expression(e->se1);
    semantic_check_relop(e->relop);
    semantic_check_simple_expression(e->se2);
    

  }

  void semantic_check_factor(struct factor_t *f)
  {
    if (f == NULL)
      error_compiler_error(line_number, "FACTOR is NULL");

    if (f->type == 1){
      if(f->data.f.next->type == 1)
      {
        error_too_many_signs(line_number);
      }
      semantic_check_sign(f->data.f.sign);
      semantic_check_factor(f->data.f.next);
    }
    else if (f->type == 2){
      semantic_check_primary(f->data.p);
    }
    else
      error_compiler_error(line_number, "WRONG FACTOR TYPE");



  }

  void semantic_check_formal_parameter_list(struct formal_parameter_section_list_t *fpsl)
  {

  }

  void semantic_check_formal_parameter_section(struct formal_parameter_section_t *fps)
  {
    //make sure all types exist.
    //this list is checked for local variables declarations/could be added to function's scope
    

  }

  void semantic_check_formal_parameter_section_list(struct formal_parameter_section_list_t *fpsl)
  {
    struct formal_parameter_section_list_t *temp = fpsl;
    while(temp != NULL)
    {
      semantic_check_formal_parameter_section(temp->fps);
      temp = temp->next;
    }

  }

  void semantic_check_func_declaration_list(struct func_declaration_list_t *fdl)
  {
    struct func_declaration_list_t * temp = fdl;
    while(temp != NULL)
    {
      cur_function_scope = temp;
      semantic_check_function_declaration(temp->fd);
      temp = temp->next;
    }

  }

  void semantic_check_function_block(struct function_block_t *fb)
  {
    //we get here
    //local variables: FIX:: These should not repeat. But can we have class / function variables with the same name?
    
    if (fb != NULL)
      //write a new one for functions only.
      semantic_check_variable_declaration_part(fb->vdl);
      semantic_check_statement_part(fb->ss);
      

  }

  

  void semantic_check_identifier_list_func(struct identifier_list_t *il)
  {
    struct func_declaration_list_t * fdl = cur_function_scope;
    struct variable_declaration_list_t *vdl_temp;
    struct identifier_list_t *idlist_temp;
    struct identifier_list_t *temp = il;

    while(temp != NULL){

      if (strcmp(temp->id, cur_function_scope->fd->fh->id) == 0)
        error_variable_name_reserved_for_function_return_value(line_number, temp->id);
      temp = temp->next;
    }
    temp = il;
    /*functions and classes can share field methods 
    this. disambiguates a name to the field name of the class*/ 
    /*we check if variable already declared in the function*/
    while (temp != NULL)
    {
      //check if field name exists in this class or any of the base classes
       
      //each class 
      if(fdl != NULL){
        if(fdl->fd == NULL)
          error_compiler_error(line_number, "SCOPE AND FUNCTION MISMATCH");
        vdl_temp = fdl->fd->fb->vdl;
        //each var list in class
        while(vdl_temp != NULL){
          idlist_temp = vdl_temp->vd->il; 
            while(idlist_temp != NULL){
              if(idlist_temp->id != temp->id && strcmp(idlist_temp->id, temp->id) == 0)
              {
                error_variable_already_declared(line_number, temp->id, vdl_temp->vd->line_number);
                exit_on_errors();
              }
              idlist_temp = idlist_temp->next;
            }
          vdl_temp = vdl_temp->next;
        }
      }

      temp = temp->next;
    }
    
  }

  void semantic_check_function_declaration(struct function_declaration_t *funcd)
  {
    if (funcd != NULL){
      line_number = funcd->line_number;
      semantic_check_function_heading(funcd->fh);
      semantic_check_function_block(funcd->fb);
    }
  }

  void semantic_check_function_designator(struct function_designator_t *fdes)
  {

  }

  void semantic_check_function_heading(struct function_heading_t *fh)
  {
    
    struct class_list_t * cl = cur_class_scope;
    struct func_declaration_list_t * fdl_temp;
    //check if function declared twice
    if (fh == NULL || fh->id == NULL)
      return;
    //recursively get this and all ancestors 
    
    while(cl != NULL){
      if(cl->cb == NULL || cl->cb->fdl == NULL)
        break;
      fdl_temp = cl->cb->fdl;
      //each function declaration list in class
      /*
      while(fdl_temp != NULL){
        if(fdl_temp->fd != NULL && fdl_temp->fd->fh != NULL && fdl_temp->fd->fh->id != NULL){ 
          if(fdl_temp->fd->fh->id != fh->id && strcmp(fdl_temp->fd->fh->id, fh->id) == 0)
            //check if this function has the same parameters and return type - then its an error.
            error_function_already_declared(line_number, fh->id, fdl_temp->fd->line_number);
            exit_on_errors();
        } 
        fdl_temp = fdl_temp->next;

      }
      */
       
      if (cl->ci != NULL && cl->ci->extend != NULL)
        cl = lookup_class_by_id(cl->ci->extend); 
      else
        cl = NULL;
    }
    semantic_check_formal_parameter_section_list(fh->fpsl);
    semantic_check_result_type(fh->res);
    

  }

  void semantic_check_function_identification(char *id)
  {

  }

  void semantic_check_identifier(char *id)
  {
    char * check = strdup(id); 
    if (id == NULL)
      printf("ID IS NULL");
    struct func_declaration_list_t *fl = cur_function_scope;
    int declared = 0;
    int is_return = 0;
    if (id == NULL)
      return;

    //how to handle True/False as primary?
    int i;
    for (i = 0; i<strlen(id);i++){
      check[i] = toupper(check[i]);
    } 
    if ((strcmp(check, "TRUE") == 0 )|| (strcmp(check, "FALSE") == 0))
      return;
    //Look up in global and local scope 
    declared = lookup_var(id); 
    
    //is it the return value?
    if (strcmp(id, fl->fd->fh->id) == 0)
      is_return = 1;



    if (is_return)
    {
      //is this variable access in lhs of assignment? 
    }
    else if (0 == declared)
    {
      error_variable_not_declared(line_number,id);
    }
    
  }

  void semantic_check_identifier_list(struct identifier_list_t *idl)
  {

    struct class_list_t * cl = cur_class_scope;
    struct variable_declaration_list_t *vdl_temp;
    struct identifier_list_t *idlist_temp;
    struct identifier_list_t *temp = idl;
    
    char * check = strdup(temp->id); 
    //for each variable in the list
    while (temp != NULL)
    {
      int i;
      for (i = 0; i<strlen(temp->id);i++){
        check[i] = toupper(check[i]);
        //printf("%c\n", check[i]);

      }
      if ((strcmp(check, "TRUE") == 0 )|| (strcmp(check, "FALSE") == 0))
        error_variable_name_invalid(line_number, temp->id);
      //check if field name exists in this class or any of the base classes
       
      //each class 
      while(cl != NULL){
        if(cl->cb == NULL)
          break;
        vdl_temp = cl->cb->vdl;
        //each var list in class
        while(vdl_temp != NULL){
          idlist_temp = vdl_temp->vd->il; 
            while(idlist_temp != NULL){
              if(idlist_temp->id != temp->id && strcmp(idlist_temp->id, temp->id) == 0)
              {
                error_variable_already_declared(line_number, temp->id, vdl_temp->vd->line_number);
                exit_on_errors();
              }
              idlist_temp = idlist_temp->next;
            }
          vdl_temp = vdl_temp->next;
        }
         
        if (cl->ci->extend != NULL)
          cl = lookup_class_by_id(cl->ci->extend); 
        else
          cl = NULL;
      }

      temp = temp->next;
    }
    
  }

  void semantic_check_if_statement(struct if_statement_t *is)
  {

  }

  void semantic_check_index_expression(struct expression_t *e)
  {
     if(e == NULL)
       return;
     semantic_check_simple_expression(e->se1);
     semantic_check_relop(e->relop);
     semantic_check_simple_expression(e->se1);
  }

  void semantic_check_index_expression_list(struct index_expression_list_t *iel)
  {

    struct index_expression_list_t * temp = iel;
    while(temp != NULL)
    {
      semantic_check_index_expression(iel->e); 
      temp=temp->next;
    }


  }

  void semantic_check_indexed_variable(struct indexed_variable_t *iv)
  {
    if (iv == NULL)
      return;
    
    semantic_check_variable_access(iv->va);
    semantic_check_index_expression_list(iv->iel);
    

  }

  void semantic_check_method_designator(struct method_designator_t *md)
  {

  }

  void semantic_check_mulop(int *op)
  {

  }

  void semantic_check_object_instantiation(struct object_instantiation_t *os)
  {

  }

  void semantic_check_params(struct actual_parameter_list_t *apl)
  {

  }

  void semantic_check_primary(struct primary_t *p)
  {
    if (p == NULL)
      error_compiler_error(line_number,"Primary is NULL");
    switch(p->type)
    {
      case 1:
      {
        semantic_check_variable_access(p->data.va);
        break;
      }
      case 2:
      {
        semantic_check_unsigned_number(p->data.un);
        break;
      }
      case 3:
      {
        semantic_check_function_designator(p->data.fd);
        break;
      }
      case 4:
      {
        semantic_check_expression(p->data.e);
        break;
      }
      case 5:
      {
        semantic_check_primary(p->data.p.next);
        break;
      }
}

  }

  void semantic_check_print_statement(struct print_statement_t *ps)
  {

  }

  void semantic_check_range(struct range_t *r)
  {

  }

  void semantic_check_relop(int *op)
  {

  }

  void semantic_check_result_type(char *id)
  {
    //Check if in existent types
    struct symbol *t;
    //t = lookupSymbol(id, 
  }

  void semantic_check_sign(int *i)
  {
    //nothing to check
  }

  void semantic_check_simple_expression(struct simple_expression_t *se)
  {
    struct simple_expression_t * temp = se;
    while (temp != NULL)
    {
      semantic_check_term(temp->t);
      semantic_check_addop(temp->addop);
      temp = temp->next;
    }


  }

  void semantic_check_statement(struct statement_t *s)
  {
    struct statement_t *temp = s;
    if(temp == NULL){
      return;
    }
    
    line_number = temp->line_number;
    switch(temp->type){
        case 1: //assignment
        {
          semantic_check_assignment_statement(s->data.as);
          break;
        }
        case 2: //statement sequence
        {
          semantic_check_statement_sequence(s->data.ss);
          break;
        }
        case 3: //if
        {
          semantic_check_if_statement(s->data.is);
          break;
        }
        case 4: //while
        {
          semantic_check_while_statement(s->data.ws);
          break;
        }
        case 5: //print
        {
          semantic_check_print_statement(s->data.ps);
          break;
        }
        default:
        {
          error_compiler_error(0,"WRONG STATEMENT TYPE");
          break;
        }
        
      }
  }

  void semantic_check_statement_part(struct statement_sequence_t *ss)
  {
    
    struct statement_sequence_t * temp = ss;
    /*statements are put into statement sequence list backwards (as is 
    everything with this parser. 
    This is an awkward fix but we rearrange the statements upside down */
    struct statement_sequence_t * cur = temp; 
    struct statement_sequence_t * next = temp->next; 
    struct statement_sequence_t * prev = NULL; 
    while(cur != NULL)
    {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    temp = prev;
    
   
    while (temp != NULL){
      semantic_check_statement(temp->s);
      temp = temp->next;
    }
    
  }

  void semantic_check_statement_sequence(struct statement_sequence_t *ss)
  {
    
    
    struct statement_sequence_t * temp = ss;
    while (temp != NULL)
    {
      semantic_check_statement(temp->s);
      temp = temp->next;
    }
  }

  void semantic_check_term(struct term_t *t)
  {
    struct term_t * temp = t;
    while (temp != NULL)
    {
      semantic_check_factor(temp->f);
      semantic_check_mulop(temp->mulop);
      temp = temp->next;
    }

  }

  void semantic_check_type_denoter(struct type_denoter_t *tden)
  {

  }

  void semantic_check_unsigned_constant(struct unsigned_number_t *un)
  {

  }

  void semantic_check_unsigned_integer(struct unsigned_number_t *un)
  {

  }

  void semantic_check_unsigned_number(struct unsigned_number_t *un)
  {

  }

  void semantic_check_value_parameter_specification(struct formal_parameter_section_t *fps)
  {

  }

  void semantic_check_variable_access(struct variable_access_t *va)
  {
    if(va == NULL)
      return;
    switch(va->type){
      case 1:{
        semantic_check_identifier(va->data.id);
        break;
      }
      case 2:{
        semantic_check_indexed_variable(va->data.iv);
        break;
      }
      case 3:{
        semantic_check_attribute_designator(va->data.ad);
        break;
      }
      case 4:{
        semantic_check_method_designator(va->data.md);
        break;
      }
      default:{
        error_compiler_error(line_number,"Wrong Variable access type");
        break;
      }
    }
  }

  void semantic_check_variable_declaration(struct variable_declaration_t *vd)
  {
    if (vd !=NULL){
      line_number = vd->line_number;
      semantic_check_identifier_list(vd->il);
    }
    
  }

  void semantic_check_variable_declaration_list(struct variable_declaration_list_t *vdl)
  {
    
    struct variable_declaration_list_t * temp  = vdl; 
    /*
    struct variable_declaration_list_t * cur = temp; 
    struct variable_declaration_list_t * next = temp->next; 
    struct variable_declaration_list_t * prev = NULL; 
    while(cur != NULL)
    {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    temp = prev;
    */

    while (temp != NULL){
      semantic_check_variable_declaration(temp->vd);
      temp = temp->next;
    }
  }

  void semantic_check_variable_declaration_part(struct variable_declaration_list_t *vdl)

  {
    
    struct variable_declaration_list_t * temp = vdl;
    /*
    struct variable_declaration_list_t * cur = temp; 
    struct variable_declaration_list_t * next = temp->next; 
    struct variable_declaration_list_t * prev = NULL; 
    while(cur != NULL)
    {
      next = cur->next;
      cur->next = prev;
      prev = cur;
      cur = next;
    }
    temp = prev;
    */
    while (temp != NULL){
      semantic_check_variable_declaration_func(temp->vd);
      temp = temp->next;
    }

  }
  void semantic_check_variable_declaration_func(struct variable_declaration_t *vd)
  {
    if (vd != NULL){
      line_number = vd->line_number;
      semantic_check_identifier_list_func(vd->il);
    }
  }

  void semantic_check_variable_parameter_specification(struct formal_parameter_section_t *fps)
  {

  }

  void semantic_check_while_statement(struct while_statement_t *ws)
  {

  }


