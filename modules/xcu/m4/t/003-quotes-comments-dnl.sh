# quoting, changequote, comments, changecom and dnl
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.003
# Quoted text is not expanded and the outer quotes are removed; changequote
# sets and restores the quote strings; comment text is copied unscanned;
# changecom changes the comment start; dnl discards through the newline.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`x',1)`x' x # x
changequote([,])dnl
[x] x [[x]]
changequote
`x' x
a dnl dropped text
c
changecom(;)dnl
; x
x
EOM
printf 'x 1 # x\nx 1 [x]\n\nx 1\na c\n; x\n1\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
