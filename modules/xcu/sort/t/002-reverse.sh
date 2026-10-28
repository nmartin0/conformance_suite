# sort -r reverses the order of comparisons.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b\nA\na\n10\n9\nB\n' >in || pc_fail "cannot create input"
pc_test
got=$(sort -r in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}a${NL}B${NL}A${NL}9${NL}10${NL}rc=0" ] || pc_fail "-r did not give descending byte order"
pc_pass
