/*
 * write.pipe_buf_unset.h - conforming variant: <limits.h> does not define
 * PIPE_BUF, and fpathconf() reports 512 (CI only; with variants/write.c).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <limits.h>, Pathname Variable Values: a value that can vary with the
 * file it applies to is left out of the header, and the application asks
 * fpathconf() or pathconf(); 512 ({_POSIX_PIPE_BUF}) is the least value
 * allowed.
 */
#include <limits.h>
#undef PIPE_BUF
