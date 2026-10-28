# get -x and -i list the excluded and included deltas
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.012
. "$PC_CORE/pc.sh"
pc_begin XCU.get.012
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
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'two\n' >>f
delta -s -y'second' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta"
get -e -s -r1.1 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e -r1.1"
printf 'branch\n' >>f
delta -s -y'branch' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta on the branch"
rm -f f
pc_test
get -r1.2 -x1.1 s.f </dev/null >out 2>err || pc_fail "get -x: nonzero status: $(cat err)"
grep -x 'Excluded:' out >/dev/null || pc_fail "no Excluded: line on standard output: $(cat out)"
grep -x 'Included:' out >/dev/null && pc_fail "Included: written without -i"
rm -f f
get -r1.2 -i1.1.1.1 s.f </dev/null >out 2>err || pc_fail "get -i: nonzero status: $(cat err)"
grep -x 'Included:' out >/dev/null || pc_fail "no Included: line on standard output: $(cat out)"
grep -x 'Excluded:' out >/dev/null && pc_fail "Excluded: written without -x"
pc_pass
