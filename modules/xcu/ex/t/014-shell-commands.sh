# ! filter and w !
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.014
# A range with ! replaces the lines by the filter output and w !cmd feeds
# lines to a command.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
2,3!sort
w
q
EOS
cat >s2.ex <<'EOS'
1,2w !cat >w.out
q
EOS
printf '%b' 'one\nthree\ntwo\nfour\nfive\n' >exp
printf '%b' 'one\ntwo\n' >exp2
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the filtered file differs: $(tr '\n' '|' <f)"
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_fail "cannot rewrite f"
ex -s f <s2.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp w.out exp2 >/dev/null 2>&1 || pc_fail "the lines written to the command differs: $(tr '\n' '|' <w.out)"
pc_pass
