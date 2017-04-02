#include <stdlib.h>
#include <stdio.h>

#include "parser.h"
#include "board.h"

bool parse(FILE *file, struct board *board) {
	init_board(board);
	
	int row = 0;
	int col = 0;
	int character;
	
	/* TODO Requires one too many characters to be typed in (unless the input is piped in). */
	while ((character = getc(file)) != EOF) {
		if (character == '\r' || character == '\n') {
			continue;
		} else if (col >= BOARD_SIZE) {
			row++;
			if (row == BOARD_SIZE) {
				break;
			}
			
			col = 0;
		}
		
		struct cell *cell = get_cell(board, row, col);
		if (character == ' ') {
			/* TODO Extract this constant from here and other places where it occurs. */
			cell->value = 511; /* 111111111 = (1 << BOARD_SIZE) - 1 */
		} else {
			int value = character - '0';
			if (value < 1 || 9 < value) {
				printf(
					"ERROR: Value %c is out of range for cell (%d, %d).\n",
					character,
					row + 1,
					col + 1
				);
				return false;
			}
			
			set_singular_value(cell, value);
		}
		
		col++;
	}
	
	return true;
}

