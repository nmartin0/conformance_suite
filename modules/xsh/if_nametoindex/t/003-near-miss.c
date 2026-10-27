/*
 * if_nametoindex() of an interface name with one character added or removed (when that names no interface) returns 0.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.if_nametoindex.003
 */
#include "if.h"

int main(void)
{
	struct if_nameindex *l;
	char longer[IF_NAMESIZE + 2], shorter[IF_NAMESIZE + 2];
	size_t len;
	int tested = 0;
	unsigned r;

	pc_begin("XSH.if_nametoindex.003");
	l = get_list();
	if (list_len(l) == 0)
		pc_notinuse("no network interface exists");
	pc_check(l[0].if_name != NULL && strlen(l[0].if_name) < IF_NAMESIZE, "the list is malformed");
	len = strlen(l[0].if_name);
	pc_test();
	if (len + 1 < IF_NAMESIZE) {
		snprintf(longer, sizeof longer, "%sx", l[0].if_name);
		if (!in_list(l, longer)) {
			r = if_nametoindex(longer);
			pc_check(r == 0, "if_nametoindex(\"%s\") returned %u, expected 0", longer, r);
			tested = 1;
		}
	}
	if (len >= 2) {
		snprintf(shorter, sizeof shorter, "%s", l[0].if_name);
		shorter[len - 1] = '\0';
		if (!in_list(l, shorter)) {
			r = if_nametoindex(shorter);
			pc_check(r == 0, "if_nametoindex(\"%s\") returned %u, expected 0", shorter, r);
			tested = 1;
		}
	}
	if_freenameindex(l);
	if (!tested)
		pc_notinuse("the variations of the first interface name are interface names themselves or too long");
	pc_pass();
}
