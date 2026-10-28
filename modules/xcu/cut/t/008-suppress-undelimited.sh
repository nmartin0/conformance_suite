# With -f a line without the delimiter is passed through unchanged, and -s suppresses such lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.008
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a:b\nnodelim\nc:d\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -d : -f 2 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}nodelim${NL}d${NL}rc=0" ] || pc_fail "line without delimiter was not passed through"
got=$(cut -d : -f 2 -s in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}d${NL}rc=0" ] || pc_fail "-s did not suppress the line without delimiter"
pc_pass
