#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdbool.h>

#include "cell.h"

#define BLOCK_SIZE 3
#define BOARD_SIZE (BLOCK_SIZE * BLOCK_SIZE)

struct board {
	struct cell cells[BOARD_SIZE][BOARD_SIZE];
};

void init_board(struct board *board);

/* TODO Move to other compilation unit - e.g. in 'parser' renamed into 'input_output'. */

void print_board(struct board *board, char all_symbol);

void print_board_debug(struct board *board);

void values(struct board *board, int values[]);

struct cell *get_cell(struct board *board, int row, int col);

bool is_singular(struct board *board);

bool is_consistent(struct board *board);
