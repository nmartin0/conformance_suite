/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
int main(void) { pc_begin("ST.c.003"); pc_check(0, "planted setup failure"); pc_test(); pc_pass(); }
