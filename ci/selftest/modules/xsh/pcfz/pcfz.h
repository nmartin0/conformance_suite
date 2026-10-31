/*
 * Fixture header for ci/mutate.sh: it redirects the name pcfz_pid to the
 * symbol getppid, as a system header may redirect a function to another
 * symbol. Nothing defines a symbol called pcfz_pid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include <sys/types.h>
pid_t pcfz_pid(void) __asm__("getppid");
