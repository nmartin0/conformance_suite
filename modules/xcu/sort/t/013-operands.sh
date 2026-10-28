# sort sorts the lines of all file operands together, reads standard input for no operand or for -, and adds a newline to an incomplete last line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.013
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.013
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'd\nb\n' >f1 && printf 'c\na\n' >f2 && printf 'y\nx' >f3 || pc_fail "cannot create input"
pc_test
got=$(sort f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}d${NL}rc=0" ] || pc_fail "two files were not sorted together"
got=$(sort <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}d${NL}rc=0" ] || pc_fail "standard input was not sorted without operands"
got=$(sort f2 - <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}d${NL}rc=0" ] || pc_fail "- did not stand for standard input"
got=$(sort f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "x${NL}y${NL}rc=0" ] || pc_fail "the incomplete last line was not completed"
pc_pass
