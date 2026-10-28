# no pending edit and stdin names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unget.003
# unget with no pending edit gives a non-zero status, and a - operand reads
# names from standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.unget.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
unget s.f </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "unget with no pending edit gave status 0"
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "get -e failed"
printf 's.f\n' | unget - >out 2>err || pc_fail "unget - : nonzero status"
[ -e p.f ] && pc_fail "the p-file survived unget -"
pc_pass
