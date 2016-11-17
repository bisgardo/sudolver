#include <stdlib.h>
#include <stdio.h>

#include "util.h"

bool is_power2(unsigned int value) {
	return (value & (value - 1)) == 0;
}

unsigned int one_bit_count(unsigned int value) {
	unsigned int count = 0;
	while (value) {
		count += value & 1;
		value >>= 1;
	}
	return count;
}

void print_binary(unsigned int value, unsigned int min_digit_count) {
	char str[] = "00000000000000000000000000000000";
	char *last = str + 31;

	char *pos = last;
	while (value) {
		if (value & 1) {
			*pos = '1';
		}
		
		pos--;
		value >>= 1;
	}

	while (min_digit_count > 32) {
		putc('0', stdout);
		min_digit_count--;
	}
	
	fputs(min(pos + 1, str + 32 - min_digit_count), stdout);
	/*printf("%s", min(pos + 1, str + 32 - min_digit_count));*/
}
