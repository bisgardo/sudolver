#pragma once
#pragma warning(disable: 4710 4711 4820) /* Disable inlining and padding warnings. */

#include <stdbool.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

bool is_power2(unsigned int value);

unsigned int one_bit_count(unsigned int value);

void print_binary(unsigned int value, unsigned int min_digit_count);

