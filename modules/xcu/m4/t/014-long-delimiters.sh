# five-character quotes and comments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.014
# Quote strings and comment strings of five characters are supported.
# MUTANT    m4.changequote_trunc1
# MUTANT    m4.changequote_trunc3
# MUTANT    m4.changequote_ltrunc4
# MUTANT    m4.changequote_rtrunc4
# MUTANT    m4.changecom_btrunc3
# MUTANT    m4.changecom_btrunc4
# MUTANT    m4.changecom_etrunc3
# MUTANT    m4.changecom_etrunc4
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(z,3)changecom(`#####',`%%%%%')#####define(y,3)%%%%%
y
####z y #y
#####a%%%%z%%%%%
changequote(`<<<<<',`>>>>>')define(<<<<<x>>>>>,<<<<<1,2>>>>>)x
<<<<<a>>>>b>>>>>
<<<<z>>>>
EOM
printf '%s\n' '#####define(y,3)%%%%%' y '####3 y #y' '#####a%%%%z%%%%%' 1,2 'a>>>>b' '<<<<3>>>>' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
pc_pass
