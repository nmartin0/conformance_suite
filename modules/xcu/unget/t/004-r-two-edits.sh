# unget -r with two outstanding edits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unget.004
. "$PC_CORE/pc.sh"
pc_begin XCU.unget.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs delta unget val sact; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
printf 'one\ntwo\n' >f
delta -s -y'two' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta"
get -e -s -r1.2 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: first edit"
mv f fA || pc_unresolved "setup: mv"
get -e -s -r1.1 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second edit"
pc_test
unget -r1.1.1.1 s.f </dev/null >out 2>err || pc_fail "unget -r1.1.1.1: nonzero status: $(cat err)"
[ "$(cat out)" = 1.1.1.1 ] || pc_fail "unget wrote '$(cat out)', expected 1.1.1.1"
sact s.f </dev/null >out 2>err
grep '1\.3' out >/dev/null || pc_fail "the other edit is no longer pending: $(cat out) $(cat err)"
grep '1\.1\.1\.1' out >/dev/null && pc_fail "the cancelled edit is still pending: $(cat out)"
pc_pass
