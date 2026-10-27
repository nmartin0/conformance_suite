/*
 * The alternative spellings expand to their operators.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.iso646.h.001
 */
#include <iso646.h>
#include <string.h>
#include "pc.h"

#define STR_(x) #x
#define STR(x) STR_(x)


int main(void)
{
	pc_begin("XBD.iso646.h.001");
	pc_test();
	pc_check(strcmp(STR(and), "&&") == 0, "and does not expand to &&");
	pc_check(strcmp(STR(and_eq), "&=") == 0, "and_eq does not expand to &=");
	pc_check(strcmp(STR(bitand), "&") == 0, "bitand does not expand to &");
	pc_check(strcmp(STR(bitor), "|") == 0, "bitor does not expand to |");
	pc_check(strcmp(STR(compl), "~") == 0, "compl does not expand to ~");
	pc_check(strcmp(STR(not), "!") == 0, "not does not expand to !");
	pc_check(strcmp(STR(not_eq), "!=") == 0, "not_eq does not expand to !=");
	pc_check(strcmp(STR(or), "||") == 0, "or does not expand to ||");
	pc_check(strcmp(STR(or_eq), "|=") == 0, "or_eq does not expand to |=");
	pc_check(strcmp(STR(xor), "^") == 0, "xor does not expand to ^");
	pc_check(strcmp(STR(xor_eq), "^=") == 0, "xor_eq does not expand to ^=");
	pc_pass();
}
