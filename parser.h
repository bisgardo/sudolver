#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdio.h>
#include <stdbool.h>

#include "board.h"

bool parse(FILE *file, struct board *board);

