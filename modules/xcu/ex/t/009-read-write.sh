# read, read from a command, write ranges and append
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.009
# r inserts a file after a line, r ! inserts command output, w with a range
# writes those lines to a new file, and w >> appends.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'I1\nI2\n' >ins || pc_unresolved "setup: cannot write ins"
printf 'pre\n' >app || pc_unresolved "setup: cannot write app"
cat >s.ex <<'EOS'
2r ins
$r !printf 'x\ny\n'
1,2w part
3,4w! >> app
w
q
EOS
printf '%b' 'one\ntwo\nI1\nI2\nthree\nfour\nfive\nx\ny\n' >exp
printf '%b' 'one\ntwo\n' >exp2
printf '%b' 'pre\nI1\nI2\n' >exp3
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
cmp part exp2 >/dev/null 2>&1 || pc_fail "the range written to part differs: $(tr '\n' '|' <part)"
cmp app exp3 >/dev/null 2>&1 || pc_fail "the appended file differs: $(tr '\n' '|' <app)"
pc_pass
