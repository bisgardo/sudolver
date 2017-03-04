#include "logging.h"

static int info_enabled = -1; /* 11...11 */
static int debug_enabled = 0; /* 00...00 */

static void enable(int *value, enum function_name id) {
	*value |= (1 << id);
}

static void disable(int *value, enum function_name id) {
	*value &= ~(1 << id);
}

static bool is_enabled(int *value, enum function_name id) {
	return *value & (1 << id);
}

void enable_info(enum function_name id) {
	enable(&info_enabled, id);
}

void disable_info(enum function_name id) {
	disable(&info_enabled, id);
}

bool is_info_enabled(enum function_name id) {
	return is_enabled(&info_enabled, id);
}

void enable_debug(enum function_name id) {
	enable(&debug_enabled, id);
}

void disable_debug(enum function_name id) {
	disable(&debug_enabled, id);
}

bool is_debug_enabled(enum function_name id) {
	return is_enabled(&debug_enabled, id);
}

void enable_all_debug(void) {
	debug_enabled = -1;
}

void enable_all_info(void) {
	info_enabled = -1;
}

void disable_all_debug(void) {
	debug_enabled = 0;
}

void disable_all_info(void) {
	info_enabled = 0;
}

