# -s and -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unget.002
# -s suppresses the SID line and -n keeps the g-file.
. "$PC_CORE/pc.sh"
pc_begin XCU.unget.002
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
unget -s -n s.f </dev/null >out 2>err || pc_fail "unget -s -n: nonzero status"
[ -s out ] && pc_fail "-s did not suppress the SID line: $(cat out)"
[ -f f ] || pc_fail "-n did not keep the g-file"
[ -e p.f ] && pc_fail "the p-file was not removed"
pc_pass
