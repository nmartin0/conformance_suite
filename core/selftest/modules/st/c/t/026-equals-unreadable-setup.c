/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stddef.h>
int main(void)
{
pc_begin("ST.c.026"); pc_make_dir("d"); (void)pc_file_equals("d", ""); pc_test(); pc_pass();
}
