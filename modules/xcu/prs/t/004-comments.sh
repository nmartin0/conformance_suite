# the comment keyword
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.004
# :C: gives the comment given to delta and :P: is not empty.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.004
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
pc_test
got=$(prs -r1.2 -d':C:' s.f </dev/null 2>/dev/null)
[ "$got" = 'add four' ] || pc_fail ":C: gave '$got'"
got=$(prs -r1.2 -d':P:' s.f </dev/null 2>/dev/null)
[ -n "$got" ] || pc_fail ":P: is empty"
pc_pass
