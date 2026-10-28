/*
 * Grammatical expressions are accepted by regcomp().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.9.5-grammar.003
 */
#include "rx.h"

static const char *const bre[] = {
	"a", "ab", ".", "[a]", "a*", "\\(a\\)", "\\(a*\\)*", "a\\{1\\}", "a\\{1,\\}",
	"a\\{1,2\\}", "a\\{0\\}", "\\(a\\)\\1", "\\(a\\)\\{2\\}", "\\(\\(a\\)\\)\\2",
	"^a", "a$", "^a$", "^$", "^", "$", "^*", "\\(*a\\)", "\\(^a\\)", "\\(a$\\)",
	"\\.", "\\*", "\\[", "\\]", "\\^", "\\$", "\\\\", "[[:alpha:]]*",
	"[[.a.]-[.z.]]", "[]a]", "[^]a]", "[a-]", "[-a]", ".*", "\\(.\\)\\1*",
	"a)", "a(", "a{", "a}", "a+", "a?", "a|b", "a]",
};
static const char *const ere[] = {
	"a", "ab", "a|b", "a|b|c", "(a)", "((a))", "(a|b)", "(a|b)*", "(a)(b)", "a*", "a+",
	"a?", "a{1}", "a{1,}", "a{1,2}", "a{0}", "(a)+", "(a*)*", "(a*)+", "(a?)?", "^",
	"$", "^a$", "(^a)", "(a$)", "^a|b$", "\\.", "\\(",
	"\\)", "\\{", "\\}", "\\|", "\\*", "\\+", "\\?", "\\^", "\\$", "\\[", "\\]", "\\\\",
	"[a]", "[^a]", "[[:alpha:]]+", "[]a]", "a}", "a]", "x{1}y{2}", "(ab){2}c", ".",
	"[[.a.]-[.z.]]", "a\\{1\\}",
};

int main(void)
{
	size_t i;

	pc_begin("XBD.9.5-grammar.003");
	pc_test();
	for (i = 0; i < RX_N(bre); i++)
		rx_ok(bre[i], 0);
	for (i = 0; i < RX_N(ere); i++)
		rx_ok(ere[i], REG_EXTENDED);
	pc_pass();
}
