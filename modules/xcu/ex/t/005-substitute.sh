# substitute, repeat and line split
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.005
# s replaces with & and groups, g replaces all, other delimiters work, a
# backslash-newline splits the line, and & repeats the last substitution.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'aaa bbb aaa\nfoo bar\nx1y2\nhello\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
1s/aaa/X/
1s/aaa/&&/
2s/\(foo\) \(bar\)/\2 \1/
3s/[0-9]/<&>/g
3s,<,[,g
4s/l/L/g
4s/e/\
/
1s/b/B/
2&
w
q
EOS
printf '%b' 'X Bbb aaaaaa\nBar foo\nx[1>y[2>\nh\nLLo\n' >exp
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
pc_pass
