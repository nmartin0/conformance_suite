/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <errno.h>
int main(void) { pc_begin("ST.c.013"); pc_test(); errno = EINVAL; pc_expect_err(-1, EEXIST, "planted"); pc_pass(); }
