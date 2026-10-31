/*
 * probe.c - calls the functions this module's fault and variant libraries
 * interposed, once each, through the headers, so the driver can read the
 * symbols the headers bind them to (see pc_symbols in posixcheck.sh).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include <stdarg.h>
#include <stdio.h>

static char pc_s[8];
static FILE *pc_f;

#ifdef PC_PROBE_vsscanf
int pc_probe_vsscanf(va_list ap) { return vsscanf(pc_s, "%d", ap); }
#endif
#ifdef PC_PROBE_vfscanf
int pc_probe_vfscanf(va_list ap) { return vfscanf(pc_f, "%d", ap); }
#endif
#ifdef PC_PROBE_vscanf
int pc_probe_vscanf(va_list ap) { return vscanf("%d", ap); }
#endif
