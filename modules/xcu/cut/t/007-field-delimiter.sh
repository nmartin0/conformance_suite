# cut -d sets the field delimiter used to split input and to join the selected output fields.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one:two:three:four\nw,x:y,z\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -d : -f 2,4 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "two:four${NL}w,x:y,z${NL}rc=0" ] || pc_fail "-d : -f 2,4 gave the wrong result"
got=$(cut -d , -f 1 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "one:two:three:four${NL}w${NL}rc=0" ] || pc_fail "-d , -f 1 gave the wrong result"
pc_pass
