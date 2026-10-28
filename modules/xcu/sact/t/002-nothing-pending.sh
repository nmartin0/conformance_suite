# no pending edit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sact.002
# With no pending edit, and after delta completes an edit, nothing is
# written to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.sact.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
sact s.f </dev/null >out 2>err
[ -s out ] && pc_fail "sact wrote to standard output with no pending edit: $(cat out)"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "get -e failed"
printf 'four\n' >>f
delta -s -y'x' s.f </dev/null >setup.out 2>&1 || pc_unresolved "delta failed"
sact s.f </dev/null >out 2>err
[ -s out ] && pc_fail "sact wrote to standard output after delta finished the edit: $(cat out)"
pc_pass
