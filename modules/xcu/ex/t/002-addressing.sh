# line addresses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.002
# Numbers, $, regular expression searches both ways, marks, offsets, the
# semicolon and % address the right lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s.ex <<'EOS'
2p
$p
1,3p
/four/p
?two?p
3ka
'ap
2;+1p
$-1p
%p
1+2p
2,$-2p
q
EOS
printf '%b' 'two\nfive\none\ntwo\nthree\nfour\ntwo\nthree\ntwo\nthree\nfour\none\ntwo\nthree\nfour\nfive\nthree\ntwo\nthree\n' >exp
pc_test
ex -s f <s.ex >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ex: exit status $rc"
cmp out exp >/dev/null 2>&1 || pc_fail "the printed lines differs: $(tr '\n' '|' <out)"
pc_pass
