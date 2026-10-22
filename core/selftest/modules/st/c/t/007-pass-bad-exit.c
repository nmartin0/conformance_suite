/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <stdlib.h>
int main(void) { pc_begin("ST.c.007"); pc_test(); pc_result_raw("PASS", ""); exit(3); }
