# yank, delete and put with named buffers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.017
# Lines yanked or deleted into a named buffer can be put back from it.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
2,3ya a
$pu a
1d b
$pu b
w
q
EOS
printf '%b' 'two\nthree\nfour\nfive\ntwo\nthree\none\n' >exp
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
pc_pass
