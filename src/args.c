#include <stdlib.h>

#include "args.h"
#include "util.h"

/* Probably OK that the executable isn't thread-safe in this single case. */
static struct arg_lit *help;
static struct arg_lit *version;
static struct arg_int *from_recursion_depth;
static struct arg_int *to_recursion_depth;
static struct arg_end *end;

static bool do_parse(char *prog_name, void *argtable[], int argc, char *argv[]) {
	/* Verify that the argtable entries were allocated sucessfully. */
	if (arg_nullcheck(argtable) != 0) {
		/* NULL entries were detected: Some allocations must have failed. */
		printf("%s: insufficient memory\n", prog_name);
		
		return false;
	}
	
	/* Parse the command line as defined by `argtable`. */
	int error_count = arg_parse(argc, argv, argtable);
	return error_count == 0;
}

static bool fetch_args(char *prog_name, struct args *args) {
	bool success = true;
	
	if (from_recursion_depth->count > 0) {
		int value = from_recursion_depth->ival[0];
		if (value > 0) {
			args->from_recursion_depth = value;
			
			/* Ensure that "to" isn't smaller than "from". */
			args->to_recursion_depth = MAX(args->to_recursion_depth, value);
		} else {
			printf("%s: `from-recursion-depth` out of range.\n", prog_name);
			success = false;
		}
	}
	
	if (to_recursion_depth->count > 0) {
		int value = to_recursion_depth->ival[0];
		if (value > 0) {
			args->to_recursion_depth = value;
			
			/* Ensure that "from" isn't larger than "to". */
			args->from_recursion_depth = MIN(args->from_recursion_depth, value);
		} else {
			printf("%s: `to-recursion-depth` out of range.\n", prog_name);
			success = false;
		}
	}
	
	return success;
}

bool parse_args(struct args *args, int argc, char *argv[]) {
	char *prog_name = "sudolver";
	
	/* Because the contents of this array need to be freed,
	   this function may not return early. */
	void *argtable[] = {
		help = arg_lit0(NULL, "help", "Display this usage message and exit"),
		version = arg_lit0(NULL, "version", "Display version information and exit"),
		from_recursion_depth = arg_int0("f", "from-depth", "DEPTH", "From recursion depth"),
		to_recursion_depth = arg_int0("t", "to-depth", "DEPTH", "To recursion depth"),
		end = arg_end(20),
	};
	
	bool success = do_parse(prog_name, argtable, argc, argv);
	
	/* Special flags `--help` and `--version` take precedence over error reporting. */
	bool suppress_errors = false;
	
	if (help->count > 0) {
		printf("Usage: %s", prog_name);
		arg_print_syntax(stdout, argtable, NULL);
		
		puts("\n\nGlossary:");
		arg_print_glossary(stdout, argtable, "  %-25s %s\n");
		
		puts("\n\nDescription:");
		puts("Parses a sudoku board from standard input one cell value at a time in row-order.");
		puts("Numbers define the value of the corresponding cell while space characters");
		puts("correspond to blank/undetermined cells. Newlines are ignored. When the parser");
		puts("has received characters for each of the 81 cells, it ignores any remaining input");
		puts("and starts the solver.");
		puts("");
		puts("The solver uses simple standard methods to try and eliminate candidate values");
		puts("from the cells that are not yet solved. If the solver gets stuck, it will pick");
		puts("a cell and make an educated guess about its value and see if this allows the");
		puts("solver to crack the puzzle. If not, the solver may either make another guess");
		puts("recursively or backtrack in order to make a different guess.");
		puts("");
		puts("The maximum number of such guesses that will be performed recursively ranges");
		puts("from 'from-recusion-depth' to 'to-recursion-depth': If the search depth");
		puts("is insuffient to find the solution, it is incremented by 1 until the whole range");
		puts("of depths has been attempted.");
		
		suppress_errors = true;
		success = false;
	}
	
	if (version->count > 0) {
		printf("'%s' Sudoku solver.\n", prog_name);
		printf("April 2017, Michael Bisgaard Olesen\n");
		
		suppress_errors = true;
		success = false;
	}
	
	if (success) {
		success = fetch_args(prog_name, args);
	} else if (!suppress_errors) {
		/* Display the error details contained in the arg_end struct. */
		arg_print_errors(stdout, end, prog_name);
		printf("Try '%s --help' for more information.\n", prog_name);
	}
	
	args->error_code = EXIT_SUCCESS;
	if (!success && !suppress_errors) {
		args->error_code = EXIT_FAILURE;
	}
	
	arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
	
	/* Return whether or not `main` should continue. */
	return success;
}
