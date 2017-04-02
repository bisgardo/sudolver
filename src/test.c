#include <stdio.h>

#include "test.h"
#include "logging.h"

static void setup_board(struct board *board, int entries[][3], int count) {
	/* Don't assume `board` to be initialized. */
	init_board(board);
	
	for (int i = 0; i < count; i++) {
		int row = entries[i][0] - 1;
		int col = entries[i][1] - 1;
		int value = entries[i][2];
		
		if (value >= 0) {
			struct cell *cell = get_cell(board, row, col);
			set_singular_value(cell, value);
			
			if (is_debug_enabled(board$setup_board)) {
				printf("DEBUG: Setting up (%d, %d) <- %d.\n", row + 1, col + 1, value);
			}
		} else if (is_debug_enabled(board$setup_board)) {
			printf("DEBUG: Ignoring set-up of (%d, %d).\n", row + 1, col + 1);
		}
	}
}

void setup_test_board_easy(struct board *board) {
	int values[][3] = {
		{ 1, 3, 3 },
		{ 1, 5, 5 },
		{ 1, 7, 8 },
		{ 2, 2, 5 },
		{ 2, 8, 7 },
		{ 3, 3, 7 },
		{ 3, 5, 1 },
		{ 3, 7, 9 },
		{ 4, 1, 2 },
		{ 4, 3, 5 },
		{ 4, 4, 8 },
		{ 4, 6, 4 },
		{ 4, 7, 1 },
		{ 4, 9, 6 },
		{ 5, 1, 1 },
		{ 5, 9, 9 },
		{ 6, 1, 3 },
		{ 6, 3, 8 },
		{ 6, 4, 1 },
		{ 6, 6, 9 },
		{ 6, 7, 7 },
		{ 6, 9, 5 },
		{ 7, 3, 1 },
		{ 7, 5, 2 },
		{ 7, 7, 6 },
		{ 8, 2, 8 },
		{ 8, 8, 9 },
		{ 9, 3, 4 },
		{ 9, 5, 9 },
		{ 9, 7, 2 }
	};
	
	setup_board(board, values, sizeof(values) / (3 * sizeof(int)));
}

void setup_test_board_medium(struct board *board) {
	int values[][3] = {
		{ 2, 1, 4 },
		{ 2, 6, 9 },
		{ 2, 8, 1 },
		{ 3, 1, 1 },
		{ 3, 2, 7 },
		{ 3, 3, 3 },
		{ 3, 6, 4 },
		{ 4, 3, 9 },
		{ 4, 5, 6 },
		{ 4, 7, 2 },
		{ 4, 8, 4 },
		{ 5, 4, 3 },
		{ 5, 6, 2 },
		{ 6, 2, 3 },
		{ 6, 3, 6 },
		{ 6, 5, 8 },
		{ 7, 4, 7 },
		{ 7, 6, 5 },
		{ 7, 7, 6 },
		{ 8, 1, 6 },
		{ 8, 4, 9 },
		{ 8, 7, 4 },
		{ 9, 1, 9 },
		{ 9, 2, 8 },
		{ 9, 7, 7 },
		{ 9, 8, 3 }
	};
	
	setup_board(board, values, sizeof(values) / (3 * sizeof(int)));
}

void setup_test_board_hard(struct board *board) {
	int values[][3] = {
		{ 1, 3, 2 },
		{ 2, 3, 6 },
		{ 2, 5, 7 },
		{ 2, 6, 4 },
		{ 2, 8, 5 },
		{ 3, 1, 1 },
		{ 3, 2, 3 },
		{ 4, 5, 3 },
		{ 4, 9, 8 },
		{ 5, 2, 5 },
		{ 5, 4, 1 },
		{ 5, 6, 6 },
		{ 5, 9, 4 },
		{ 6, 2, 4 },
		{ 6, 5, 9 },
		{ 6, 8, 6 },
		{ 6, 9, 3 },
		{ 7, 7, 4 },
		{ 8, 2, 1 },
		{ 8, 6, 7 },
		{ 8, 9, 2 },
		{ 9, 4, 3 },
		{ 9, 5, 5 },
		{ 9, 6, 8 },
		{ 9, 8, 9 }
	};
	
	setup_board(board, values, sizeof(values) / (3 * sizeof(int)));
}

void setup_test_board_insane(struct board *board) {
	int values[][3] = {
		{ 1, 1, 8 },
		{ 2, 3, 3 },
		{ 2, 4, 6 },
		{ 3, 2, 7 },
		{ 3, 5, 9 },
		{ 3, 7, 2 },
		{ 4, 2, 5 },
		{ 4, 6, 7 },
		{ 5, 5, 4 },
		{ 5, 6, 5 },
		{ 5, 7, 7 },
		{ 6, 4, 1 },
		{ 6, 8, 3 },
		{ 7, 3, 1 },
		{ 7, 8, 6 },
		{ 7, 9, 8 },
		{ 8, 3, 8 },
		{ 8, 4, 5 },
		{ 8, 8, 1 },
		{ 9, 2, 9 },
		{ 9, 7, 4 }
	};
	
	setup_board(board, values, sizeof(values) / (3 * sizeof(int)));
}

