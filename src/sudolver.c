#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "solve.h"
#include "test.h"
#include "logging.h"
#include "parser.h"
#include "args.h"

int default_from_recursion_depth = 5;
int default_to_recursion_depth = 9;

int main(int argc, char *argv[]) {
	struct args args;
	args.from_recursion_depth = default_from_recursion_depth;
	args.to_recursion_depth = default_to_recursion_depth;
	
	bool parse_args_success = parse_args(&args, argc, argv);
	if (!parse_args_success) {
		return EXIT_FAILURE;
	}
	
	enable_all_info();
	
	struct board board;
	/*setup_test_board_easy(&board);*/
	/*setup_test_board_medium(&board);*/
	/*setup_test_board_hard(&board);*/
	setup_test_board_insane(&board);
	
	/* Parse board from input. */
	/*bool parse_board_success = parse(stdin, &board);
	if (!parse_board_success) {
		return EXIT_FAILURE;
	}*/
	
	puts("Initial board:");
	print_board(&board, '_');
	
	struct board *solved_board = NULL;
	
	clock_t begin_time = clock();
	
	/* Perform first solution attempt without any optimistic guesses. */
	bool consistent = solve(&board);
	if (!consistent) {
		puts("Board is inconsistent and cannot be solved!");
		return EXIT_FAILURE;
	} else if (is_singular(&board)) {
		/* Consistent and singular means solved. */
		solved_board = &board;
	} else {
		/*puts("Board after initial attempt to solve:");
		print_board(&board, 0);*/
		
		/* Invoke recursive solver. */
		for (int recursion_depth = args.from_recursion_depth; recursion_depth <= args.to_recursion_depth; recursion_depth++) {
			printf("Max recursion depth: %d.\n", recursion_depth);
			/* TODO Refactor `recursive_solve` such that it doesn't start out with a useless call
			        to solve, but calls it from within the loop. */
			solved_board = recursive_solve(&board, 1, recursion_depth);
			if (solved_board) {
				break;
			}
		}
	}
	
	clock_t end_time = clock();
	
	printf("#attempts: %d.\n", solve_call_count());
	
	if (solved_board) {
		printf("SOLVED in %.3f s! Result:\n", (float) (end_time - begin_time) / CLOCKS_PER_SEC);
		print_board(solved_board, 0);
	} else {
		/* TODO Make sure that we have enough information to determine if a solution may exist or
		        not (i.e., if the whole tree of possibilities were exhausted). */
		puts("Not solved...");
		print_board(&board, '*');
	}
	
	return EXIT_SUCCESS;
}
