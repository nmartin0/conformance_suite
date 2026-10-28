# get -e -b and the b flag
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.013
. "$PC_CORE/pc.sh"
pc_begin XCU.get.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v get >/dev/null 2>&1 || pc_notinuse "get is not available"
command -v admin >/dev/null 2>&1 || pc_notinuse "admin is not available"
command -v delta >/dev/null 2>&1 || pc_notinuse "delta is not available"
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
rm -f f
pc_test
# no b flag: -b is ignored
get -e -b s.f </dev/null >out 2>err || pc_fail "get -e -b: nonzero status: $(cat err)"
grep -x 'new delta 1\.2' out >/dev/null || pc_fail "without the b flag -b was not ignored: $(cat out)"
rm -f f p.f
# with the b flag the leaf gets a branch
admin -fb s.f </dev/null >out 2>err || pc_unresolved "admin -fb failed: $(cat err)"
get -e -b s.f </dev/null >out 2>err || pc_fail "get -e -b with the b flag: nonzero status: $(cat err)"
grep -x 'new delta 1\.1\.1\.1' out >/dev/null || pc_fail "b flag and leaf did not give a branch: $(cat out)"
rm -f f p.f
# without -b the b flag changes nothing
get -e s.f </dev/null >out 2>err || pc_fail "get -e: nonzero status: $(cat err)"
grep -x 'new delta 1\.2' out >/dev/null || pc_fail "get -e without -b did not make 1.2: $(cat out)"
pc_pass
