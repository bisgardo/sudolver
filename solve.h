#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include "board.h"

#include <stdbool.h>

int solve_call_count(void);

bool clean(struct board *board);

void process_row(struct board *board, int row, int value);

void process_col(struct board *board, int col, int value);

void process_block(struct board *board, int block_row, int block_col, int value);

bool solve(struct board *board);

int min_candidate_count(struct board *b);

struct board *recursive_solve(struct board *b, int level, int max_level);

