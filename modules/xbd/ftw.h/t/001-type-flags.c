/*
 * FTW_F, FTW_D, FTW_DNR, FTW_DP, FTW_NS, FTW_SL and FTW_SLN have seven
 * different values.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.ftw.h.001
 */
#include "pc.h"
#include <ftw.h>

int main(void)
{
	pc_begin("XBD.ftw.h.001");
#if !defined(FTW_F) || !defined(FTW_D) || !defined(FTW_DNR) || !defined(FTW_DP) || !defined(FTW_NS) || !defined(FTW_SL) || !defined(FTW_SLN)
	pc_unresolved("a constant is missing (the header check reports it): FTW_F FTW_D FTW_DNR FTW_DP FTW_NS FTW_SL FTW_SLN");
#else
	{
	struct { const char *n; long long v; } e[8];
	int n = 0, i, j;

	e[n].n = "FTW_F"; e[n++].v = (long long)(FTW_F);
	e[n].n = "FTW_D"; e[n++].v = (long long)(FTW_D);
	e[n].n = "FTW_DNR"; e[n++].v = (long long)(FTW_DNR);
	e[n].n = "FTW_DP"; e[n++].v = (long long)(FTW_DP);
	e[n].n = "FTW_NS"; e[n++].v = (long long)(FTW_NS);
	e[n].n = "FTW_SL"; e[n++].v = (long long)(FTW_SL);
	e[n].n = "FTW_SLN"; e[n++].v = (long long)(FTW_SLN);
	if (n < 7)
		pc_unresolved("fewer than 7 of the constants are defined");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			pc_check(e[i].v != e[j].v, "%s and %s are both %lld", e[i].n, e[j].n, e[i].v);
		}
	pc_pass();
	}
#endif
}
