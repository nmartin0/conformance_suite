# A - operand stands for standard input in place of file1 or file2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.009
. "$PC_CORE/pc.sh"
pc_begin XCU.join.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\n' >f1 && printf 'a x\nb y\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join - f2 <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file1 did not read standard input"
got=$(join f1 - <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file2 did not read standard input"
pc_pass
