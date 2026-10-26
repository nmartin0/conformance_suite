/*
 * pcfy.shimmed.h - fixture build shim: declares pcfy_other() and gives the
 * "system's" own (wrong) behaviour (CI only).
 * posixcheck: shim
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
static int pcfy_other(void)
{
	return 7;
}
