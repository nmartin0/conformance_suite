# -p and -s
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.get.001
# -p writes the text to standard output and moves the informative lines to
# standard error, and -s with -p drops them.
. "$PC_CORE/pc.sh"
pc_begin XCU.get.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
get -p s.f </dev/null >out 2>err || pc_fail "get -p: nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "get -p wrote the text: $(od -c out | head -3)"
grep -x '1\.1' err >/dev/null || pc_fail "no SID line on standard error: $(cat err)"
grep -x '3 lines' err >/dev/null || pc_fail "no line count on standard error: $(cat err)"
get -p -s s.f </dev/null >out 2>err || pc_fail "get -p -s: nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "get -p -s wrote the text: $(od -c out | head -3)"
grep -x '1\.1' err >/dev/null && pc_fail "-s did not suppress the informative lines: $(cat err)"
grep -x '3 lines' err >/dev/null && pc_fail "-s did not suppress the informative lines: $(cat err)"
[ -e f ] && pc_fail "get -p created a g-file"
pc_pass
