# pax -s
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.010
# "-s /^f/g/" renames f1 to g1 in write mode and in read mode (-s ,f,h,
# makes h1), the first expression that succeeds applies, and a trailing p
# flag writes "OLD >> NEW" to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
pc_test
pax -w -s '/^f/g/' f1 >a 2>err </dev/null || pc_fail "write with -s: nonzero status"
pax <a >o 2>err || pc_fail "list: nonzero status"
[ "$(cat o)" = g1 ] || pc_fail "-s /^f/g/ named the member '$(cat o)'"
pax -w f1 >b 2>err </dev/null || pc_fail "pax -w: nonzero status"
pax -r -s ',f,h,' <b >o 2>err || pc_fail "read with -s: nonzero status"
[ -f h1 ] || pc_fail "read mode -s did not create h1"
pax -w -s ',x,y,' -s ',f,k,' f1 >c 2>err </dev/null || pc_fail "write with two -s: nonzero status"
pax <c >o 2>err || pc_fail "list: nonzero status"
[ "$(cat o)" = k1 ] || pc_fail "two -s expressions named the member '$(cat o)'"
pax -w -s ',f,m,p' f1 >d 2>e </dev/null || pc_fail "write with the p flag: nonzero status"
grep -x 'f1 >> m1' e >/dev/null || pc_fail "no 'f1 >> m1' line on standard error"
pc_pass
