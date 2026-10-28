/*
 * pcl.h - names of the locales the system offers.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 */
#ifndef PCL_H
#define PCL_H

#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PCL_MAX 300
#define PCL_LEN 64
static char pcl_name[PCL_MAX][PCL_LEN];
static int pcl_count;

/*
 * "POSIX" first, then each name `locale -a` writes that setlocale()
 * accepts. Leaves the global locale at "C". Call in the setup phase.
 */
static void pcl_collect(void)
{
	char line[256];
	FILE *p;
	size_t n;

	pcl_count = 0;
	(void)strcpy(pcl_name[pcl_count++], "POSIX");
	p = popen("locale -a 2>/dev/null", "r");
	if (p == NULL)
		return;
	while (pcl_count < PCL_MAX && fgets(line, sizeof line, p) != NULL) {
		n = strlen(line);
		while (n > 0 && (line[n - 1] == '\n' || line[n - 1] == '\r'))
			line[--n] = '\0';
		if (n == 0 || n >= PCL_LEN || strcmp(line, "C") == 0 ||
		    strcmp(line, "POSIX") == 0)
			continue;
		if (setlocale(LC_ALL, line) == NULL)
			continue;
		(void)strcpy(pcl_name[pcl_count++], line);
	}
	(void)pclose(p);
	(void)setlocale(LC_ALL, "C");
}

#endif
