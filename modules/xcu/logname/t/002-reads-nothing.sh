# logname does not read its input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logname.002
. "$PC_CORE/pc.sh"
pc_begin XCU.logname.002
printf 'first\nsecond\n' >in || pc_unresolved "setup: cannot create a file"
pc_test
{
	logname >/dev/null 2>&1
	IFS= read -r a
	IFS= read -r b
} <in
[ "$a" = first ] || pc_fail "after logname the first line read was '$a', wanted first"
[ "$b" = second ] || pc_fail "after logname the second line read was '$b', wanted second"
pc_pass
