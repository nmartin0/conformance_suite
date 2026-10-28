# five-character quotes and comments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.014
# Quote strings and comment strings of five characters are supported.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
changecom(`#####',`%%%%%')#####define(y,3)%%%%%
y
changequote(`<<<<<',`>>>>>')define(<<<<<x>>>>>,<<<<<1,2>>>>>)x
EOM
printf '%s\ny\n1,2\n' '#####define(y,3)%%%%%' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
pc_pass
