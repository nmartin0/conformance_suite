# mkstemp that cannot create a file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.016
# When mkstemp cannot create the file m4 writes a diagnostic, continues, the
# text is empty and the final status is non-zero.
# MUTANT    m4.mkstemp_fail_no_diag
# MUTANT    m4.mkstemp_fail_text
# MUTANT    m4.mkstemp_fail_abort
# MUTANT    m4.nodiag_all
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a[mkstemp(`%s/nodir/xXXXXXX'"'"')]b\nend\n' "$PWD" >in
printf 'a[]b\nend\n' >exp
pc_test
m4 in >out 2>err </dev/null
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
[ -s err ] || pc_fail "no diagnostic on standard error"
pc_pass
