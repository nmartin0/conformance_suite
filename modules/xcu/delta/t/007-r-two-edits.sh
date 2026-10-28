# delta -r chooses between two outstanding edits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.007
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin delta prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'one\ntwo\n' >f
delta -s -y'two' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta"
get -e -s -r1.2 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: first outstanding edit"
mv f fA || pc_unresolved "setup: mv"
get -e -s -r1.1 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second outstanding edit"
mv f fB || pc_unresolved "setup: mv"
printf 'one\ntwo\nA\n' >fA
printf 'one\nB\n' >fB
cp fA f
pc_test
delta -s -r1.2 -y'edit A' s.f </dev/null >out 2>err || pc_fail "delta -r1.2 (SID given to get): nonzero status: $(cat err)"
cp fB f
delta -s -r1.1.1.1 -y'edit B' s.f </dev/null >out 2>err || pc_fail "delta -r1.1.1.1 (SID to be made): nonzero status: $(cat err)"
get -s -p -r1.3 s.f </dev/null >got 2>err || pc_fail "cannot get 1.3: $(cat err)"
cmp got fA >/dev/null 2>&1 || pc_fail "1.3 is not the first edit: $(cat got)"
get -s -p -r1.1.1.1 s.f </dev/null >got 2>err || pc_fail "cannot get 1.1.1.1: $(cat err)"
cmp got fB >/dev/null 2>&1 || pc_fail "1.1.1.1 is not the second edit: $(cat got)"
pc_pass
