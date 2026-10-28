# delta -g ignores deltas when the new level is accessed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.008
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin delta prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'a\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'x\na\n' >f
delta -s -y'prepend x' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta 1.2"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'x\na\nz\n' >f
delta -s -y'append z' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta 1.3"
get -s -p -r1.3 s.f </dev/null >got 2>/dev/null
printf 'x\na\nz\n' >exp
cmp got exp >/dev/null 2>&1 || pc_unresolved "setup: 1.3 is not as expected"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'x\na\nz\nw\n' >f
pc_test
delta -s -g1.2 -y'append w' s.f </dev/null >out 2>err || pc_fail "delta -g: nonzero status: $(cat err)"
get -s -p -r1.4 s.f </dev/null >got 2>err || pc_fail "cannot get 1.4: $(cat err)"
printf 'a\nz\nw\n' >exp
cmp got exp >/dev/null 2>&1 || pc_fail "1.4 does not ignore delta 1.2: $(cat got)"
get -s -p -r1.3 s.f </dev/null >got 2>err || pc_fail "cannot get 1.3: $(cat err)"
printf 'x\na\nz\n' >exp
cmp got exp >/dev/null 2>&1 || pc_fail "1.3 changed: $(cat got)"
pc_pass
