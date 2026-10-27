/*
 * 0.0.0.0 gives zero and 255.255.255.255 gives all ones.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.inet_addr.007
 */
#include "ia.h"

int main(void)
{
	pc_begin("XSH.inet_addr.007");
	pc_test();
	pc_check(inet_addr("0.0.0.0") == 0, "inet_addr(\"0.0.0.0\") is not 0");
	pc_check(inet_addr("255.255.255.255") == (in_addr_t)-1, "inet_addr(\"255.255.255.255\") is not all ones");
	pc_pass();
}
