# undo and undo of undo
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.007
# u reverses the last change and a second u reverses the undo.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
2d
u
w
q
EOS
cp f orig
cat >s2.ex <<'EOS'
3d
u
u
w
q
EOS
printf '%b' 'one\ntwo\nfour\nfive\n' >exp2
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f orig >/dev/null 2>&1 || pc_fail "u did not restore the deleted line"
ex -s f <s2.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp2 >/dev/null 2>&1 || pc_fail "after undo of undo differs: $(tr '\n' '|' <f)"
pc_pass
