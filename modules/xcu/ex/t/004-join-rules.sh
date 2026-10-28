# join spacing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.004
# j drops leading spaces, puts two spaces after a period, none after a
# trailing blank or before a closing parenthesis, and j! joins unchanged.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'end.\n  next\ntrail \nmore\nx\n)y\na\n  b\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
1,2j
2,3j
3,4j
4,5j!
w
q
EOS
printf '%b' 'end.  next\ntrail more\nx)y\na  b\n' >exp
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
pc_pass
