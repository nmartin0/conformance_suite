# Selecting bytes or characters beyond the end of a line is not an error; a line with nothing selected gives an empty line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.005
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\nwxyz\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -b 3-9 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "c${NL}xyz${NL}rc=0" ] || pc_fail "-b 3-9 gave the wrong result"
got=$(cut -c 9 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}${NL}rc=0" ] || pc_fail "-c 9 did not give two empty lines with status 0"
pc_pass
