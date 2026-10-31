/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stddef.h>
int main(void)
{
pc_begin("ST.c.022"); (void)pc_file_size("nosuch"); pc_test(); pc_pass();
}
