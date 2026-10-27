/*
 * Installing a locale does not change another thread's locale.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.uselocale.005
 */
#include "lc.h"

#include <pthread.h>
#include <unistd.h>

static int gate[2];
static int saw_global, saw_posix;

static void *other(void *arg)
{
	char c;

	(void)arg;
	if (read(gate[0], &c, 1) != 1)
		return NULL;
	saw_global = uselocale((locale_t)0) == LC_GLOBAL_LOCALE;
	saw_posix = strcmp(nl_langinfo(CODESET), lc_c0) == 0;
	return NULL;
}

int main(void)
{
	pthread_t t;
	locale_t objl;
	char c = 'x';

	pc_begin("XSH.uselocale.005");
	lc_init();
	lc_need();
	objl = newlocale(LC_ALL_MASK, lc_loc, (locale_t)0);
	if (objl == (locale_t)0)
		pc_unresolved("newlocale() could not make an object from the second locale");
	if (setlocale(LC_ALL, "C") == NULL)
		pc_unresolved("setlocale(LC_ALL, \"C\") failed");
	if (pipe(gate) != 0)
		pc_unresolved("pipe() failed");
	if (pthread_create(&t, NULL, other, NULL) != 0)
		pc_unresolved("pthread_create() failed");
	pc_test();
	if (uselocale(objl) == (locale_t)0)
		pc_unresolved("uselocale() could not install the object");
	if (write(gate[1], &c, 1) != 1)
		pc_unresolved("write() failed");
	if (pthread_join(t, NULL) != 0)
		pc_unresolved("pthread_join() failed");
	pc_check(saw_global, "the other thread's uselocale(0) did not return LC_GLOBAL_LOCALE after this thread installed a locale");
	pc_check(saw_posix, "the other thread did not see the POSIX code set after this thread installed a locale");
	pc_check(uselocale((locale_t)0) == objl, "this thread's locale is no longer the installed object");
	pc_check(strcmp(nl_langinfo(CODESET), "UTF-8") == 0, "this thread lost the second locale's code set");
	(void)uselocale(LC_GLOBAL_LOCALE);
	freelocale(objl);
	pc_pass();
}
