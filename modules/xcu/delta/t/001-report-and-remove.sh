# delta report, g-file removal and new version
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.001
# delta prints the new SID and the inserted, deleted and unchanged line
# counts; the g-file is removed, the new version is what get retrieves, the
# old one remains available, and another get -e is allowed afterwards.
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
printf 'four\n' >>f || pc_unresolved "setup: cannot edit f"
pc_test
delta -y'add four' s.f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "delta: exit status $rc"
printf '1.2\n1 inserted\n0 deleted\n3 unchanged\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the delta report differs: $(cat out)"
[ -e f ] && pc_fail "the g-file was not removed"
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the new version text: $(od -c out | head -3)"
get -s -p -r1.1 s.f </dev/null >out 2>err || pc_fail "get -r1.1: nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the old version text: $(od -c out | head -3)"
get -e -s s.f </dev/null >out 2>err || pc_fail "get -e after delta: nonzero status"
pc_pass
