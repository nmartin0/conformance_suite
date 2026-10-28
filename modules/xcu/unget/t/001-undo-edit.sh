# unget undoes get -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unget.001
# unget prints the SID that would have been made, removes the p-file and
# g-file, and allows a new get -e.
. "$PC_CORE/pc.sh"
pc_begin XCU.unget.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
pc_test
unget s.f </dev/null >out 2>err || pc_fail "unget: nonzero status"
[ "$(cat out)" = 1.2 ] || pc_fail "unget wrote '$(cat out)', expected 1.2"
[ -e p.f ] && pc_fail "the p-file was not removed"
[ -e f ] && pc_fail "the g-file was not removed"
get -e -s s.f </dev/null >out 2>err || pc_fail "get -e after unget: nonzero status"
pc_pass
