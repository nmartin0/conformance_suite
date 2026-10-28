# fold -s breaks after the last blank that fits in the width, and acts as plain fold when the segment has no blank.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.004
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aaa bbb ccc\n' >in1 || pc_fail "cannot create input"
printf 'abcdefghij\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(fold -s -w 9 in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "aaa bbb ${NL}ccc${NL}rc=0" ] || pc_fail "-s -w 9 did not break after the last blank"
got=$(fold -s -w 4 in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcd${NL}efgh${NL}ij${NL}rc=0" ] || pc_fail "-s changed a segment with no blank"
pc_pass
