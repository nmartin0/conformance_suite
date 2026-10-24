/*
 * exit.failure_125.h - conforming variant: EXIT_FAILURE is 125 (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <stdlib.h> (CX) requires EXIT_FAILURE to be an integer constant
 * from 1 to 125 inclusive; 125 is the largest such value. exit() passes
 * any value on, so the library needs no change.
 */
#include <stdlib.h>
#undef EXIT_FAILURE
#define EXIT_FAILURE 125
