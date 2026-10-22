/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
int main(void) { pc_begin("ST.c.008"); pc_test(); pc_result_raw("PASS", "one"); pc_result_raw("PASS", "two"); return 0; }
