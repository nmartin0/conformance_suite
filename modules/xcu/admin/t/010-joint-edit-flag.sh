# admin -fj allows concurrent get -e on one SID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.010
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for u in get admin prs; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
printf 'one\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e"
mv f f1 || pc_unresolved "setup: mv"
get -e -s s.f </dev/null >setup.out 2>&1 && pc_unresolved "setup: a second get -e was allowed without the j flag"
rm -f f
admin -fj s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin -fj"
pc_test
get -e -s s.f </dev/null >out 2>err || pc_fail "second get -e with the j flag failed: $(cat err)"
[ -f f ] || pc_fail "no g-file for the second edit"
pc_pass
