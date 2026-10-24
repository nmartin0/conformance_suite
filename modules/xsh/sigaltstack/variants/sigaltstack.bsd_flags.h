/*
 * sigaltstack.bsd_flags.h - conforming variant: SS_DISABLE is 4 (CI only;
 * with variants/sigaltstack.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <signal.h> only names SS_ONSTACK and SS_DISABLE as symbolic
 * constants; their values are the implementation's (the BSDs use 1 and 4,
 * Linux 1 and 2). Included before the test; the preload library makes
 * sigaltstack() agree, so Linux's 2 is an ordinary invalid flag here.
 */
#include <signal.h>
#undef SS_DISABLE
#define SS_DISABLE 4
