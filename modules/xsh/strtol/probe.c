/*
 * probe.c - calls the functions this module's fault and variant libraries
 * interposed, once each, through the headers, so the driver can read the
 * symbols the headers bind them to (see pc_symbols in posixcheck.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include <stdlib.h>

static char pc_s[8];
static char *pc_e;

#ifdef PC_PROBE_strtol
long pc_probe_strtol(void) { return strtol(pc_s, &pc_e, 10); }
#endif
#ifdef PC_PROBE_strtoll
long long pc_probe_strtoll(void) { return strtoll(pc_s, &pc_e, 10); }
#endif
