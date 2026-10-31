/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stddef.h>
int main(void)
{
pc_begin("ST.c.019"); pc_make_file("f", "x"); pc_test(); (void)pc_file_equals("nosuch", "x"); pc_pass();
}
