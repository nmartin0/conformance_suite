#define _POSIX_C_SOURCE 202405L
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wordexp.h>
int main(void)
{
	const char *cases[] = { "a b  c", "'a b'", "$HOME", "x*y", "~", "" };
	size_t i;
	for (i = 0; i < sizeof cases / sizeof cases[0]; i++) {
		wordexp_t w; int r;
		memset(&w, 0, sizeof w); errno = 0;
		r = wordexp(cases[i], &w, 0);
		printf("wordexp(\"%s\") = %d (errno %d) words %zu", cases[i], r, errno, r ? 0 : w.we_wordc);
		if (!r && w.we_wordc) printf(" first '%s'", w.we_wordv[0]);
		printf("\n");
		if (!r) wordfree(&w);
	}
	{ wordexp_t w; int r; memset(&w, 0, sizeof w); r = wordexp("$(echo hi)", &w, WRDE_NOCMD); printf("NOCMD cmdsub = %d (WRDE_CMDSUB=%d)\n", r, WRDE_CMDSUB); }
	printf("system(echo) = %d\n", system("echo from-sh"));
	return 0;
}
