# uniq writes one copy of each run of identical adjacent lines and leaves non-adjacent repeats alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.001
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\na\nc\nc\nc\n' >in || pc_fail "cannot create input"
pc_test
got=$(uniq in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}a${NL}c${NL}rc=0" ] || pc_fail "adjacent duplicates were not collapsed exactly"
got=$(uniq <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}a${NL}c${NL}rc=0" ] || pc_fail "standard input was not read without operands"
pc_pass
