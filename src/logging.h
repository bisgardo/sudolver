#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdio.h>
#include <stdbool.h>

enum function_name {
	/* solve.h */
	solve$clean,
	solve$process_row,
	solve$process_col,
	solve$process_block,
	solve$solve,
	solve$recursive_solve,
	/* board.h */
	board$setup_board,
	board$destroy_board,
	board$is_consistent,
	board$is_solved,
	/* cell.h */
	cell$remove_from_range,
	cell$remove_from_block,
	cell$remove_from_row,
	cell$remove_from_col
};

void enable_info(enum function_name id);

void disable_info(enum function_name id);

bool is_info_enabled(enum function_name id);

void enable_debug(enum function_name id);

void disable_debug(enum function_name id);

bool is_debug_enabled(enum function_name id);

void enable_all_debug(void);

void enable_all_info(void);

void disable_all_debug(void);

void disable_all_info(void);

