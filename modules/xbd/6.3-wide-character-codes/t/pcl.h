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
#include <wchar.h>

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

/*
 * 1 if the encoding of the current locale has a shift state: some sequence
 * of up to three bytes completes a unit (a character or a shift sequence)
 * and leaves the conversion state other than the initial one. A multibyte
 * character that is complete returns the state to the initial one, so the
 * ordinary multibyte encodings give 0.
 */
static inline int pcl_shifting(void)
{
	int a, b, c;
	mbstate_t s0, s1, s2;
	wchar_t w;
	size_t r;
	char x;

	for (a = 1; a < 256; a++) {
		memset(&s0, 0, sizeof s0);
		x = (char)a;
		r = mbrtowc(&w, &x, 1, &s0);
		if (r == (size_t)-1)
			continue;
		if (r != (size_t)-2) {
			if (!mbsinit(&s0))
				return 1;
			continue;
		}
		for (b = 1; b < 256; b++) {
			s1 = s0;
			x = (char)b;
			r = mbrtowc(&w, &x, 1, &s1);
			if (r == (size_t)-1)
				continue;
			if (r != (size_t)-2) {
				if (!mbsinit(&s1))
					return 1;
				continue;
			}
			for (c = 1; c < 256; c++) {
				s2 = s1;
				x = (char)c;
				r = mbrtowc(&w, &x, 1, &s2);
				if (r != (size_t)-1 && r != (size_t)-2 && !mbsinit(&s2))
					return 1;
			}
		}
	}
	return 0;
}

#endif
