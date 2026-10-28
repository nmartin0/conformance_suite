# Selecting a field a line does not have is not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.009
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a:b\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -d : -f 5 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}rc=0" ] || pc_fail "-f 5 on a two-field line did not give an empty line with status 0"
got=$(cut -d : -f 1,5 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}rc=0" ] || pc_fail "-f 1,5 on a two-field line did not give a"
pc_pass
