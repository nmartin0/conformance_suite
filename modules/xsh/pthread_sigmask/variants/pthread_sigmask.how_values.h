/*
 * pthread_sigmask.how_values.h - conforming variant: SIG_BLOCK is 1,
 * SIG_UNBLOCK is -12345 and SIG_SETMASK is 3 (CI only; with
 * variants/pthread_sigmask.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <signal.h> names the three how values as symbolic constants and
 * says nothing about their values beyond being distinct, so any int will
 * do, including negative ones and ones a test might pick as "surely
 * invalid". Included before the test; the preload library makes
 * pthread_sigmask() and sigprocmask() agree, and the C library's own
 * values become ordinary invalid ones.
 */
#include <signal.h>
#undef SIG_BLOCK
#undef SIG_UNBLOCK
#undef SIG_SETMASK
#define SIG_BLOCK 1
#define SIG_UNBLOCK (-12345)
#define SIG_SETMASK 3
