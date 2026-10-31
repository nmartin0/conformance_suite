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
static int pc_i;
static FILE *pc_f;

#ifdef PC_PROBE_sscanf
int pc_probe_sscanf(void) { return sscanf(pc_s, "%d", &pc_i); }
#endif
#ifdef PC_PROBE_fscanf
int pc_probe_fscanf(void) { return fscanf(pc_f, "%d", &pc_i); }
#endif
#ifdef PC_PROBE_scanf
int pc_probe_scanf(void) { return scanf("%d", &pc_i); }
#endif
#ifdef PC_PROBE_vsscanf
int pc_probe_vsscanf(va_list ap) { return vsscanf(pc_s, "%d", ap); }
#endif
#ifdef PC_PROBE_vfscanf
int pc_probe_vfscanf(va_list ap) { return vfscanf(pc_f, "%d", ap); }
#endif
