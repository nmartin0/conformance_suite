/*
 * wait.status_encoding.h - conforming variant: the wait status value has a
 * layout of its own (CI only; with variants/wait.c, which says why the
 * standard allows it and gives the layout).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Both headers that define the macros are included first, so that a later
 * include of either one keeps these definitions.
 */
#include <stdlib.h>
#include <sys/wait.h>
#undef WIFEXITED
#undef WEXITSTATUS
#undef WIFSIGNALED
#undef WTERMSIG
#undef WCOREDUMP
#undef WIFSTOPPED
#undef WSTOPSIG
#undef WIFCONTINUED
#define WIFEXITED(s) (((s) & ~0xff) == 0)
#define WEXITSTATUS(s) ((s) & 0xff)
#define WIFSIGNALED(s) ((((s) >> 24) & 0xff) == 1)
#define WTERMSIG(s) (((s) >> 8) & 0xff)
#define WCOREDUMP(s) (((s) >> 20) & 1)
#define WIFSTOPPED(s) ((((s) >> 24) & 0xff) == 2)
#define WSTOPSIG(s) (((s) >> 8) & 0xff)
#define WIFCONTINUED(s) ((((s) >> 24) & 0xff) == 3)
