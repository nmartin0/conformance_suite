# -r, -e and -l
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.prs.003
# -r picks one delta (the latest with no SID), -e adds all earlier ones and
# -l all later ones, and a cutoff before every delta with -e selects
# nothing.
. "$PC_CORE/pc.sh"
pc_begin XCU.prs.003
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
pc_test
sel() { prs "$@" -d':I:\n' s.f </dev/null 2>/dev/null | sort | tr '\n' ' '; }
[ "$(sel -r1.2)" = '1.2 ' ] || pc_fail "-r1.2 selected '$(sel -r1.2)'"
[ "$(sel -e -r1.2)" = '1.1 1.2 ' ] || pc_fail "-e -r1.2 selected '$(sel -e -r1.2)'"
[ "$(sel -l -r1.2)" = '1.2 1.3 ' ] || pc_fail "-l -r1.2 selected '$(sel -l -r1.2)'"
[ "$(sel -r)" = '1.3 ' ] || pc_fail "-r with no SID selected '$(sel -r)'"
[ "$(sel -e -c68)" = '1.1 1.2 1.3 ' ] || pc_fail "-e -c68 selected '$(sel -e -c68)'"
[ "$(sel -e -c69)" = '' ] || pc_fail "-e -c69 selected '$(sel -e -c69)'"
pc_pass
