# fold -w width breaks lines into segments of at most that many columns and leaves shorter lines alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.002
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\nabcd\nab\n' >in || pc_fail "cannot create input"
pc_test
got=$(fold -w 4 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${NL}efgh${NL}ij${NL}abcd${NL}ab${NL}rc=0" ] || pc_fail "-w 4 gave the wrong result"
got=$(fold -w 10 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}abcd${NL}ab${NL}rc=0" ] || pc_fail "-w 10 changed lines that fit"
got=$(fold -w 3 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}def${NL}ghi${NL}j${NL}abc${NL}d${NL}ab${NL}rc=0" ] || pc_fail "-w 3 gave the wrong result"
pc_pass
