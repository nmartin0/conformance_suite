# -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.005
# Without -a only existing deltas are shown; with -a a removed delta is
# shown with type R.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.005
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
delta -s -y'add four' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: delta failed"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second get -e failed"
printf 'five\n' >>f || pc_unresolved "setup: cannot edit f"
delta -s -y'add five' s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: second delta failed"
rmdel -r1.3 s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: rmdel failed"
pc_test
sel() { prs "$@" -d':I:/:DT:\n' s.f </dev/null 2>/dev/null | sort | tr '\n' ' '; }
[ "$(sel -l -r1.1)" = '1.1/D 1.2/D ' ] || pc_fail "without -a: '$(sel -l -r1.1)'"
[ "$(sel -a -l -r1.1)" = '1.1/D 1.2/D 1.3/R ' ] || pc_fail "with -a: '$(sel -a -l -r1.1)'"
pc_pass
