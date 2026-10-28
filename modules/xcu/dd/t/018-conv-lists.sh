# dd conv= lists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.018
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Conversions may be listed in one conv= or spread over several.
printf 'abcd' >in || pc_fail "cannot write in"
printf 'BADC' >exp || pc_fail "cannot write exp"
pc_test
dd if=in of=out conv=ucase,swab </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=ucase,swab exited with status $rc"
cmp -s exp out || pc_fail "conv=ucase,swab did not both apply"
dd if=in of=out conv=swab,ucase </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=swab,ucase exited with status $rc"
cmp -s exp out || pc_fail "conv=swab,ucase did not both apply"
dd if=in of=out conv=ucase conv=swab </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=ucase conv=swab exited with status $rc"
cmp -s exp out || pc_fail "conv=ucase conv=swab did not both apply"
pc_pass
