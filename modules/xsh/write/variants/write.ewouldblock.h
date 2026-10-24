/*
 * write.ewouldblock.h - conforming variant: EWOULDBLOCK is distinct from
 * EAGAIN (CI only; with variants/write.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <errno.h> says EWOULDBLOCK may have the same value as EAGAIN, so it
 * may also have another one; XSH write() and send() let a non-blocking
 * socket write that would wait fail with either. Included before the
 * test, so the test sees this value; the preload library returns it.
 */
#include <errno.h>
#undef EWOULDBLOCK
#define EWOULDBLOCK 1001
