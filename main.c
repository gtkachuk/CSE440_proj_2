/*
 * main.c
 *
 * Implements an object oriented pascal compiler
 */
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include "shared.h"
#include "symtab.h"
#include "rulefuncs.h"
#include "semantic.h"
#include "optimization.h"
#include "code.h"

/* Flags if any errors occured */
int error_flag = 0;

/* Holds the command-line arguments */
struct args_t cmdArgs;

/* This is the data structure we are left with (the parse tree) after
   yyparse() is done . */
extern struct program_t *program;

extern void yyparse();
//extern void optimize(struct program_t *p);

/* -----------------------------------------------------------------------
 * Printout on error message and exit
 * ----------------------------------------------------------------------- 
 */
void exit_on_errors()
{
  if (error_flag == 1) {
    printf("Errors detected. Exiting.\n");
    exit(-1);
  }
}



/* ----------------------------------------------------------------------- 
 * Prints a usage banner
 * ----------------------------------------------------------------------- 
 */
void print_usage()
{
  printf("%s: Usage: %s [-v] [-exitsem] [-O] [-ir] [-free] [-fcf] [-fvn] [-fgre] [--help]\n",
	  PROGRAMNAME, PROGRAMNAME);
}


/* ----------------------------------------------------------------------- 
 * parses the command-line arguments to main
 * ----------------------------------------------------------------------- 
 */
void parse_command_line_arguments(int argc, char **argv, struct args_t *args)
{
  int i = 1;

  if (argc < 1) {
    print_usage();
    exit(1);
  }

  memset(args, 0, sizeof(struct args_t));

  while (i < argc) {
    if (strcmp(argv[i], "-v") == 0 ) {
      args->verbose = 1;
    } else if (strcmp(argv[i], "--help") == 0 ||
	       strcmp(argv[i], "-h") == 0 ) { 
      print_usage();
      exit(3);
    } else if (strcmp(argv[i], "-exitsem") == 0) {
      args->exit_after_sem = 1;
    } else if (strcmp(argv[i], "-ir") == 0) {
      args->printIr = 1;
    } else if (strcmp(argv[i], "-lir") == 0) {
      args->printLir = 1;
    } else if (strcmp(argv[i], "-fcf") == 0) {
      args->optimize_cf = 1;
    } else if (strcmp(argv[i], "-free") == 0) {
      args->optimize_ree = 1;
    } else if (strcmp(argv[i], "-fvn") == 0) {
      args->optimize_vn = 1;
    } else if (strcmp(argv[i], "-fgre") == 0) {
      args->optimize_gre = 1;
    } else if (strcmp(argv[i], "-O") == 0) {
      args->optimize_cf = 1;
      args->optimize_ree = 1;
      args->optimize_vn = 1;
      args->optimize_gre = 1;
    } else {
      fprintf(stderr, "Invalid argument '%s'\n", argv[i]);
      print_usage();
      exit(1);
    }
    i++;
  }

}

/* -----------------------------------------------------------------------
 * main function, where all hell breaks loose
 * ----------------------------------------------------------------------- 
 */
int main(int argc, char **argv)
{
	parse_command_line_arguments(argc, argv, &cmdArgs);

	symtab_init();

	yyparse();

	//maybe you can do this when creating the basic blocks
	//goto_label_populate(program);
	//print_code(program);
	//variable_table_print();
	//expression_table_print();
	label_table_print();
	optimize(program);
	return 0;
}
