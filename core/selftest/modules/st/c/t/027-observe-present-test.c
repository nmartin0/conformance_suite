/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stddef.h>
int main(void)
{
pc_begin("ST.c.027"); pc_make_file("f", "abc"); pc_test();
	pc_check(pc_file_equals("f", "abc") == 1, "equal contents");
	pc_check(pc_file_equals("f", "abd") == 0, "different contents");
	pc_check(pc_file_equals("f", "ab") == 0, "shorter wanted");
	pc_check(pc_file_equals("f", "abcd") == 0, "longer wanted");
	pc_check(pc_file_size("f") == 3, "size");
	pc_check(pc_file_mode("f") == 0644, "mode");
	pc_pass();
}
