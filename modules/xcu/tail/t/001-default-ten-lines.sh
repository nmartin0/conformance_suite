# With no options tail writes the last ten lines of its input, or all of it when shorter.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.001
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 11 12 >f12 || pc_fail "cannot create input"
printf '%s\n' a b c >f3 || pc_fail "cannot create input"
pc_test
got=$(tail f12 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}4${NL}5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}11${NL}12${NL}rc=0" ] || pc_fail "tail did not write exactly the last ten lines"
got=$(tail f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}rc=0" ] || pc_fail "tail did not copy a short file whole"
pc_pass
