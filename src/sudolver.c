#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "solve.h"
#include "test.h"
#include "logging.h"
#include "parser.h"

int default_from_recursion_depth = 5;
int default_to_recursion_depth = 9;

int board_number = 0;

int main(int argc, char *argv[]) {
	int from_recursion_depth = default_from_recursion_depth;
	int to_recursion_depth = default_to_recursion_depth;
	
	enable_all_info();
	
	if (argc == 2) {
		to_recursion_depth = atoi(argv[1]);
		if (from_recursion_depth > to_recursion_depth) {
			from_recursion_depth = to_recursion_depth;
		}
	} else if (argc == 3) {
		from_recursion_depth = atoi(argv[1]);
		to_recursion_depth = atoi(argv[2]);
	}
	
	int error_status = 0;
	if (from_recursion_depth <= 0) {
		puts("ERROR: 'from-recursion-depth' could not be parsed or was non-positive.");
		error_status = -1;
	}
	if (to_recursion_depth <= 0 || BOARD_SIZE * BOARD_SIZE < to_recursion_depth) {
		puts("ERROR: 'to-recursion-depth' could not be parsed or was outside of range.");
		error_status = -1;
	}
	if (error_status == 0 && from_recursion_depth > to_recursion_depth) {
		printf(
			"ERROR: 'to-recursion-depth' = %d > %d = 'to-recursion-depth'.\n",
			to_recursion_depth,
			from_recursion_depth
		);
		error_status = -2;
	}
	
	if (error_status != 0) {
		puts("");
	}
	
	if (argc > 3 || error_status != 0) {
		printf(
			"Usage: %s [[from-recursion-depth:%d] to-recursion-depth:%d]\n\n",
			argv[0],
			default_from_recursion_depth,
			default_to_recursion_depth
		);
		
		puts("Description:");
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
		
		return error_status;
	}
	
	/* Parse board from input. */
	
	struct board board;
	/*setup_test_board_easy(&board);*/
	/*setup_test_board_medium(&board);*/
	/*setup_test_board_hard(&board);*/
	/*setup_test_board_insane(&board);*/
	
	bool success = parse(stdin, &board);
	if (!success) {
		return EXIT_FAILURE;
	}
	
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
		for (int recursion_depth = from_recursion_depth; recursion_depth <= to_recursion_depth; recursion_depth++) {
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

