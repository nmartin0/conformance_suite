# When head stops before the end of a seekable input it leaves the file offset just after the last byte it used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.007
. "$PC_CORE/pc.sh"
pc_begin XCU.head.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >in || pc_fail "cannot create input"
printf 'abcdef\n' >in2 || pc_fail "cannot create input"
pc_test
got=$( (head -n 1; cat) <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}2${NL}3${NL}rc=0" ] || pc_fail "after head -n 1 the offset was not just past the first line"
got=$( (head -c 3; cat) <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdef${NL}rc=0" ] || pc_fail "after head -c 3 the offset was not just past the third byte"
pc_pass
