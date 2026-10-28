# A line beginning with a space is written without its first character and otherwise unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.001
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf ' hello\n  indented\n \n' >in || pc_fail "cannot create input"
pc_test
got=$(asa <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello${NL} indented${NL}${NL}rc=0" ] || pc_fail "space lines were not written without the first character"
got=$(asa in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello${NL} indented${NL}${NL}rc=0" ] || pc_fail "a file operand gave a different result from standard input"
pc_pass
