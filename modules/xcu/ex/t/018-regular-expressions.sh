# word boundaries, intervals and ignorecase
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.018
# \< and \> match word edges, \{n\} matches an interval, and set ignorecase
# matches regardless of case.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'the other\nthen\nbathe\naaa\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
set noap
%s/\<the\>/X/
4s/a\{2\}/Z/
set ws
set ic
/BATHE/p
w
q
EOS
printf '%b' 'X other\nthen\nbathe\nZa\n' >exp
printf '%b' 'bathe\n' >expout
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp f exp >/dev/null 2>&1 || pc_fail "the edited file differs: $(tr '\n' '|' <f)"
cmp out expout >/dev/null 2>&1 || pc_fail "the ignorecase search result differs: $(tr '\n' '|' <out)"
pc_pass
