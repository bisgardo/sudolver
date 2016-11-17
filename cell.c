#include <stdio.h>

#include "cell.h"
#include "board.h"
#include "logging.h"
#include "util.h"

int value_mask(int value) {
	return 1 << (value - 1);
}

int get_singular_value(struct cell *cell) {
	int value = cell->value;
	if (!is_power2(value)) {
		return 0;
	}
	
	// TODO Can be optimized some clever way...
	int res = 0;
	while (value) {
		res++;
		value >>= 1;
	}
	return res;
}

void set_singular_value(struct cell *cell, int value) {
	cell->value = value_mask(value);
	cell->dirty = true;
}

static bool remove_from_cell(struct cell *cell, int value) {
	int mask = value_mask(value);
	if ((cell->value & mask) == 0) {
		// Value was not in the set.
		return false;
	}
	
	cell->value &= ~mask;
	cell->dirty = true;
	
	return true;
}

unsigned int get_candidate_count(struct cell *cell) {
	int value = cell->value;
	return one_bit_count(value);
}

bool has_candidate(struct cell *cell, int candidate) {
	int mask = value_mask(candidate);
	return cell->value & mask;
}

/* TODO Pack ranges into struct(s)? */
static void remove_from_range(struct board *board, struct cell *cell, int value, int row_from, int row_to, int col_from, int col_to) {
	for (int row = row_from; row < row_to; row++) {
		for (int col = col_from; col < col_to; col++) {
			struct cell *bc = get_cell(board, row, col);
			if (bc != cell) {
				bool removed = remove_from_cell(bc, value);
				if (removed && is_debug_enabled(cell$remove_from_range)) {
					printf(
						"INFO: (%d, %d): Removed %d as possible value for cell (%d, %d).\n",
						cell->row + 1,
						cell->col + 1,
						value,
						row + 1,
						col + 1
					);
				}
			}
		}
	}
}

void remove_from_block(struct board *board, struct cell *cell, int value) {
	int row = cell->row;
	int col = cell->col;
	
	int block_row = row / BLOCK_SIZE;
	int block_col = col / BLOCK_SIZE;
	
	int row_from = block_row * BLOCK_SIZE;
	int row_to = row_from + BLOCK_SIZE;
	
	int col_from = block_col * BLOCK_SIZE;
	int col_to = col_from + BLOCK_SIZE;
	
	remove_from_range(board, cell, value, row_from, row_to, col_from, col_to);
}

void remove_from_row(struct board *board, struct cell *cell, int value) {
	int row = cell->row;
	remove_from_range(board, cell, value, row, row + 1, 0, 9);
}

void remove_from_col(struct board *board, struct cell *cell, int value) {
	int col = cell->col;
	remove_from_range(board, cell, value, 0, 9, col, col + 1);
}
