#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdbool.h>

struct board;

struct cell {
	int row;
	int col;
	
	/* TODO Could just use a 16-bit int. */
	int value;
	bool dirty;
};

int value_mask(int value);

int get_singular_value(struct cell *cell);

void set_singular_value(struct cell *cell, int value);

unsigned int get_candidate_count(struct cell *cell);

bool has_candidate(struct cell *cell, int candidate);

void remove_from_block(struct board *board, struct cell *cell, int value);

void remove_from_row(struct board *board, struct cell *cell, int value);

void remove_from_col(struct board *board, struct cell *cell, int value);

