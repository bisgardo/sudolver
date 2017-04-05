#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdbool.h>

#include "../lib/argtable3/argtable3.h"

struct args {
	int from_recursion_depth;
	int to_recursion_depth;
	int error_code;
};

bool parse_args(struct args *args, int argc, char *argv[]);
