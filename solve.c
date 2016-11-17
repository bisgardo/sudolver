#include <stdlib.h>

#include "solve.h"
#include "logging.h"

int call_count = 0;

int solve_call_count(void) {
	return call_count;
}

bool clean(struct board *board) {
	bool cleaned = false;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			if (!cell->dirty) {
				if (is_debug_enabled(solve$clean)) {
					printf("DEBUG: Skipping non-dirty cell (%d, %d).\n", row + 1, col + 1);
				}
				continue;
			}
			
			if (is_debug_enabled(solve$clean)) {
				printf("DEBUG: Cleaning dirty cell (%d, %d).\n", row + 1, col + 1);
			}
			
			cell->dirty = false;
			cleaned = true;
			
			int value = get_singular_value(cell);
			if (value > 0) {
				if (is_debug_enabled(solve$clean)) {
					printf(
						"DEBUG: Dirty cell (%d, %d) has singular value %d.\n",
						row + 1,
						col + 1,
						value
					);
				}
				remove_from_block(board, cell, value);
				remove_from_row(board, cell, value);
				remove_from_col(board, cell, value);
			}
		}
	}
	
	return cleaned;
}

/* TODO Un-duplicate the following two functions. */

void process_row(struct board *board, int row, int value) {
	struct cell *singular_cell = NULL;
	int mask = value_mask(value);
	for (int col = 0; col < BOARD_SIZE; col++) {
		struct cell *cell = get_cell(board, row, col);
		
		if (cell->value == mask) {
			/* Cell is already singular, which means that no other cells in the row can have that
			   value. */
			return;
		}
		
		if (cell->value & mask) {
			if (is_debug_enabled(solve$process_row)) {
				printf("DEBUG: Cell (%d, %d) has value %d.\n", row + 1, col + 1, value);
			}
			
			/* Cell has the value. */
			if (singular_cell != NULL) {
				/* Cell is not the only cell with that value. */
				return;
			}
			singular_cell = cell;
		}
	}
	
	if (singular_cell != NULL) {
		set_singular_value(singular_cell, value);
	}
}

void process_col(struct board *board, int col, int value) {
	struct cell *singular_cell = NULL;
	int mask = value_mask(value);
	for (int row = 0; row < BOARD_SIZE; row++) {
		struct cell *cell = get_cell(board, row, col);
		
		if (cell->value == mask) {
			/* Cell is already singular, which means that no other cells in the row can have that
			   value. */
			return;
		}
		
		if (cell->value & mask) {
			if (is_debug_enabled(solve$process_col)) {
				printf("DEBUG: Cell (%d, %d) has value %d.\n", row + 1, col + 1, value);
			}
			
			/* Cell has the value. */
			if (singular_cell != NULL) {
				/* Cell is not the only cell with that value. */
				return;
			}
			singular_cell = cell;
		}
	}
	
	if (singular_cell != NULL) {
		set_singular_value(singular_cell, value);
	}
}

void process_block(struct board *board, int block_row, int block_col, int value) {
	struct cell *singular_cell = NULL;
	int mask = value_mask(value);
	
	int row_from = 3 * block_row;
	int row_to = row_from + 3;
	int col_from = 3 * block_col;
	int col_to = col_from + 3;
	
	for (int row = row_from; row < row_to; row++) {
		for (int col = col_from; col < col_to; col++) {
			struct cell *cell = get_cell(board, row, col);
			
			if (cell->value == mask) {
				/* Cell is already singular, which means that no other cells in the row can have
				   that element. */
				return;
			}
			
			if (cell->value & mask) {
				if (is_debug_enabled(solve$process_block)) {
					printf("DEBUG: Cell (%d, %d) has value %d.\n", row + 1, col + 1, value);
				}
				
				/* Cell has the value. */
				if (singular_cell != NULL) {
					/* Cell is not the only cell with that value. */
					return;
				}
				singular_cell = cell;
			}
		}
	}
	
	if (singular_cell != NULL) {
		set_singular_value(singular_cell, value);
	}
}

/* Returns boolean value of whether the board is consistent. */
bool solve(struct board *board) {
	call_count++;
	while (true) {
		int iteration_count = 0;
		while (clean(board)) {
			iteration_count++;
		}
		
		if (is_debug_enabled(solve$solve)) {
			printf("DEBUG: Iteration count: %d.\n", iteration_count);
		}
		
		if (!is_consistent(board)) {
			return false;
		} else if (iteration_count == 0) {
			return true;
		}
		
		/* For each row, and col, and block, check if there is just one cell that may contain that
		   value. If so, set that value, thus giving the cleaning something to work with in the
		   next iteration. */
		for (int value = 1; value <= BOARD_SIZE; value++) {
			for (int index = 0; index < BOARD_SIZE; index++) {
				process_row(board, index, value);
				process_col(board, index, value);
			}
			
			for (int block_row = 0; block_row < BLOCK_SIZE; block_row++) {
				for (int block_col = 0; block_col < BLOCK_SIZE; block_col++) {
					process_block(board, block_row, block_col, value);
				}
			}
		}
	}
}

int min_candidate_count(struct board *board) {
	int min_candidate_count = BOARD_SIZE;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			int count = get_candidate_count(cell);
			if (1 < count && count < min_candidate_count) {
				min_candidate_count = count;
			}
		}
	}
	return min_candidate_count;
}

/* TODO Pass info down (or on the way back?) to reveal "path" to solution.
        Proposal: "Unrecurse" function by letting the stack be a linked chain of objects that
                  contain the board (since we know the max recursion depth this could actually also
                  be an array) in a local variable. This allows us to inspect the entire stack at
				  all times, as well as not destroy it when we return. */
struct board *recursive_solve(struct board *board, int level, int max_level) {
	if (is_info_enabled(solve$recursive_solve)) {
		printf("INFO: Attempting to solve board at recursion level %d.\n", level);
	}
	
	bool consistent = solve(board);
	if (is_info_enabled(solve$recursive_solve)) {
		printf("Board after %d. recursive attempt to solve:\n", level);
		print_board(board, 0);
	}
	
	/* No point continuing if `board` is inconsistent. */
	if (!consistent) {
		return NULL;
	}
	
	if (is_singular(board)) {
		/* Consistent and singular means solved: Return solution. */
		/* TODO Remove this temporary hack: Copy board to heap. Note that it is currently never
		        freed again. */
		struct board *result = (struct board *) malloc(sizeof(struct board));
		*result = *board;
		return result;
	}
	
	/* Solver got stuck, but board is not inconsistent. Brute-force by need by making
	   optimistic guess and see if that works. */
	
	/* Honor recursion depth limit. */
	if (level == max_level) {
		if (is_info_enabled(solve$recursive_solve)) {
			puts("INFO: Hit max recursion level - abandoning branch.");
		}
		return NULL;
	}
	
	/* TODO Alternative strategy: *Remove* candiate from cell - much higher probablity of guessing
	        correctly. */
	/* TODO Parallelize: Fork-join branches. The code is completely CPU-bound and threads will have
	        no shared state, so this should be extremely fast. */
	
	/* The lowest number of candidate values (greater than 1). */
	int candidate_count = min_candidate_count(board);
	
	if (is_debug_enabled(solve$recursive_solve)) {
		printf("DEBUG: Min candidate count: %d.\n", candidate_count);
	}
	
	/* Iterate over all cells that contain `candidate_count` candidates. */
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			int count = get_candidate_count(cell);
			if (count == candidate_count) {
				int value = cell->value;
				/* For each of the `candidate_count` candidates, optimistically assume that the
				   cell has this value and see if this leads to a solution. */
				
				for (int candidate = 1; candidate <= BOARD_SIZE; candidate++) {
					if (value & value_mask(candidate)) {
						if (is_info_enabled(solve$recursive_solve)) {
							printf(
								"INFO: Optimistically fixing cell (%d, %d) to its candidate value %d.\n",
								row + 1,
								col + 1, candidate
							);
						}
						
						struct board copy = *board;
						set_singular_value(get_cell(&copy, row, col), candidate);
						
						struct board *solved = recursive_solve(&copy, level + 1, max_level);
						if (solved) {
							return solved;
						}
						
						if (is_info_enabled(solve$recursive_solve)) {
							printf(
								"INFO: Optimistic fix failed. Proceeding at recursion level %d.\n",
								level
							);
						}
					}
				}
			}
		}
	}
	
	/* There are no more guesses to be made. */
	return NULL;
}
