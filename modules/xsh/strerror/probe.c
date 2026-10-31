/*
 * probe.c - calls the functions this module's fault and variant libraries
 * interposed, once each, through the headers, so the driver can read the
 * symbols the headers bind them to (see pc_symbols in posixcheck.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include <string.h>

static char pc_b[8];

#ifdef PC_PROBE_strerror_r
int pc_probe_strerror_r(void) { return (int)(long)strerror_r(0, pc_b, sizeof pc_b); }
#endif
