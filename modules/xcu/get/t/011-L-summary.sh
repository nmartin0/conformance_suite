# get -L writes a delta summary and sends informative lines to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.011
. "$PC_CORE/pc.sh"
pc_begin XCU.get.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v get >/dev/null 2>&1 || pc_notinuse "get is not available"
command -v admin >/dev/null 2>&1 || pc_notinuse "admin is not available"
command -v delta >/dev/null 2>&1 || pc_notinuse "delta is not available"
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin"
rm -f f
pc_test
get -L s.f </dev/null >out 2>err || pc_fail "get -L: nonzero status: $(cat err)"
grep -E '^...[[:blank:]]1\.1[[:blank:]]' out >/dev/null || pc_fail "no summary line for 1.1 on standard output: $(cat out)"
grep -x '3 lines' out >/dev/null && pc_fail "informative line on standard output with -L"
grep -x '3 lines' err >/dev/null || pc_fail "informative line missing from standard error: $(cat err)"
rm -f f
get -L -s s.f </dev/null >out 2>err || pc_fail "get -L -s: nonzero status: $(cat err)"
grep -x '3 lines' err >/dev/null && pc_fail "-s did not suppress the informative lines"
grep -x '3 lines' out >/dev/null && pc_fail "-s did not suppress the informative lines"
pc_pass
