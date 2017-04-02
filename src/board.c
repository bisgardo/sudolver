#include <stdlib.h>
#include <assert.h>

#include "board.h"
#include "logging.h"
#include "util.h"

void init_board(struct board *board) {
	int value = 511; /* 111111111 = (1 << BOARD_SIZE) - 1 */
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			cell->row = row;
			cell->col = col;
			cell->dirty = false;
			cell->value = value;
		}
	}
}

void print_board(struct board *board, char all_values_symbol) {
	int all_values = 511; /* 111111111 = (1 << BOARD_SIZE) - 1 */
	
	/* TODO Use 'putc' instead of 'printf'. */
	
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			int cell_value = cell->value;
			
			int spaces = 19;
			if (cell_value == all_values && all_values_symbol) {
				putc(all_values_symbol, stdout);
				spaces--;
			} else {
				spaces -= 2;
				
				printf("[");
				bool first = true;
				int mask = 1;
				for (int value = 1; value <= BOARD_SIZE; value++) {
					if (cell_value & mask) {
						/* Has value; print it. */
						if (first) {
							first = false;
						} else {
							printf(",");
							spaces--;
						}
						printf("%d", value);
						spaces--;
					}
					mask <<= 1;
				}
				printf("]");
			}
			while (spaces--) {
				printf(" ");
			}
			printf("\t");
		}
		printf("\n");
	}
}

void print_board_debug(struct board *board) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			print_binary(cell->value, 9);
			putc('\t', stdout);
			putc('\t', stdout);
		}
		putc('\n', stdout);
	}
}

bool values(struct board *board, int values[]) {
	int index = 0;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			int value = get_singular_value(cell);
			if (value == 0) {
				return false;
			}
			
			values[index++] = value;
		}
	}
	return true;
}

struct cell *get_cell(struct board *board, int row, int col) {
	return &board->cells[row][col];
}

bool is_singular(struct board *board) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			int value = get_singular_value(cell);
			if (value == 0) {
				/* Non-singular value means not solved. */
				if (is_debug_enabled(board$is_solved)) {
					printf("DEBUG: Non-singular value on cell (%d, %d).\n", row + 1, col + 1);
				}
				return false;
			}
		}
	}
	
	return true;
}

bool is_consistent(struct board *board) {
	int row_values[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int col_values[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int block_values[][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
	
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			struct cell *cell = get_cell(board, row, col);
			
			int value = cell->value;
			if (value == 0) {
				return false;
			}
			
			int mask = value;
			
			row_values[row] |= mask;
			col_values[col] |= mask;
			block_values[row / BLOCK_SIZE][col / BLOCK_SIZE] |= mask;
		}
	}
	
	int all_values = 511; /* 111111111 = (1 << BOARD_SIZE) - 1 */
	
	/* Check that all rows and values are fully covered. */
	for (int index = 0; index < BOARD_SIZE; index++) {
		if (row_values[index] != all_values) {
			if (is_debug_enabled(board$is_consistent)) {
				printf("DEBUG: Not all values are covered on row %d.\n", index + 1);
			}
			return false;
		}
		
		if (col_values[index] != all_values) {
			if (is_debug_enabled(board$is_consistent)) {
				printf("DEBUG: Not all values are covered on col %d.\n", index + 1);
			}
			return false;
		}
	}
	
	/* Check that all blocks are fully covered. */
	for (int block_row = 0; block_row < BLOCK_SIZE; block_row++) {
		for (int block_col = 0; block_col < BLOCK_SIZE; block_col++) {
			if (block_values[block_row][block_col] != all_values) {
				if (is_debug_enabled(board$is_consistent)) {
					printf(
						"DEBUG: Not all values are covered in block (%d, %d).\n",
						block_row + 1,
						block_col + 1
					);
				}
				return false;
			}
		}
	}
	
	return true;
}

