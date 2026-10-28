# uniq -u writes only the lines that are not repeated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.004
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\nc\nc\nc\nd\n' >in || pc_fail "cannot create input"
pc_test
got=$(uniq -u in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}d${NL}rc=0" ] || pc_fail "-u did not write exactly the unrepeated lines"
pc_pass
