/*
 * write.pipe_buf_512.h - conforming variant: {PIPE_BUF} is 512 (CI only;
 * with variants/write.c, which makes fpathconf() agree).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <limits.h>, Pathname Variable Values: the least {PIPE_BUF} allowed
 * is {_POSIX_PIPE_BUF}, 512. A system whose pipes keep larger writes
 * atomic as well still conforms.
 */
#include <limits.h>
#undef PIPE_BUF
#define PIPE_BUF 512
