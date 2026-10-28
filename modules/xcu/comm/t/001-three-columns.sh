# comm writes lines only in file1 unindented, lines only in file2 after one tab, and lines in both after two tabs, in sorted order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.001
. "$PC_CORE/pc.sh"
pc_begin XCU.comm.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a\nb\nc\nd\n' >f1 && printf 'b\nc\ne\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(comm f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${T}${T}b${NL}${T}${T}c${NL}d${NL}${T}e${NL}rc=0" ] || pc_fail "the three columns are wrong"
pc_pass
