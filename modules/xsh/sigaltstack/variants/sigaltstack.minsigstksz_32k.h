/*
 * sigaltstack.minsigstksz_32k.h - conforming variant: MINSIGSTKSZ and
 * SIGSTKSZ are both 32768 (CI only; with variants/sigaltstack.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <signal.h> and XSH sigaltstack() give MINSIGSTKSZ as the least
 * stack a handler needs and SIGSTKSZ as a default size for the usual case,
 * with no values: a system with large signal frames may need 32 KiB for
 * both. Included before the test; the preload library makes sigaltstack()
 * fail with ENOMEM below this MINSIGSTKSZ, as its ERRORS section requires.
 */
#include <signal.h>
#undef MINSIGSTKSZ
#undef SIGSTKSZ
#define MINSIGSTKSZ 32768
#define SIGSTKSZ 32768
