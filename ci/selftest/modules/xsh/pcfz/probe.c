/*
 * probe.c - fixture: calls the interposed function once, through the
 * header, so the driver can read the symbol the object references.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#include "pcfz.h"

#ifdef PC_PROBE_pcfz_pid
void pc_probe_pcfz_pid(void) { (void)pcfz_pid(); }
#endif
