#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include "board.h"

void setup_test_board_easy(struct board *board);

void setup_test_board_medium(struct board *board);

void setup_test_board_hard(struct board *board);

void setup_test_board_insane(struct board *board);
