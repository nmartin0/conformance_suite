# quoting, changequote, comments, changecom and dnl
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.003
# Quoted text is not expanded and the outer quotes are removed; changequote
# sets and restores the quote strings; comment text is copied unscanned;
# changecom changes the comment start; dnl discards through the newline.
# MUTANT    m4.quote_keep_outer
# MUTANT    m4.quote_ignored
# MUTANT    m4.comment_scanned
# MUTANT    m4.comment_end_newline
# MUTANT    m4.changequote_ignored
# MUTANT    m4.changequote_noarg_noop
# MUTANT    m4.changequote_noarg_disable
# MUTANT    m4.changequote_right_unchanged
# MUTANT    m4.changecom_ignored
# MUTANT    m4.changecom_noarg_noop
# MUTANT    m4.changecom_keeps_hash
# MUTANT    m4.changecom_end_ignored
# MUTANT    m4.dnl_noop
# MUTANT    m4.dnl_keeps_newline
# MUTANT    m4.dnl_word_only
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
# x
changecom(`<!--',`-->')dnl
<!-- x
x -->x
changecom
# x
<!-- x
EOM
printf 'x 1 # x\nx 1 [x]\n\nx 1\na c\n; x\n1\n# 1\n<!-- x\nx -->1\n\n# 1\n<!-- 1\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "output differs: $(cat out)"
pc_pass
