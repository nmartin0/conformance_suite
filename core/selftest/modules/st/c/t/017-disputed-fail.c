/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
int main(void) { pc_begin("ST.c.017"); pc_test(); pc_check(0, "planted failure"); pc_pass(); }
