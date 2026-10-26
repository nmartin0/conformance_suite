/*
 * pcfy.c - fixture: the fault "wrong" is applied by variants/pcfy.provides.h;
 * this library only names it for lint (CI only).
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
static const char *pcfy_faults[] = { "wrong" };
const char **pcfy_fault_names(void);
const char **pcfy_fault_names(void)
{
	return pcfy_faults;
}
