# With no options sort writes the lines in ascending order of the whole line, by byte value in the POSIX locale.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'b\nA\na\n10\n9\nB\n' >in || pc_fail "cannot create input"
pc_test
got=$(sort in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "10${NL}9${NL}A${NL}B${NL}a${NL}b${NL}rc=0" ] || pc_fail "lines were not in ascending byte order"
pc_pass
