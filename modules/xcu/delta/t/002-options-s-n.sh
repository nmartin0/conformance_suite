# -s and -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.002
# -s suppresses the report and -n keeps the edited g-file.
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.002
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
delta -s -n -y'add four' s.f </dev/null >out 2>err || pc_fail "delta -s -n: nonzero status"
[ -s out ] && pc_fail "-s did not suppress the report: $(cat out)"
[ -f f ] || pc_fail "-n did not keep the g-file"
printf 'one\ntwo\nthree\nfour\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "the kept g-file differs"
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'one\ntwo\nthree\nfour\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the new version text: $(od -c out | head -3)"
pc_pass
