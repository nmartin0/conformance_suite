# include and sinclude
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.009
# include gives the contents of a file; sinclude of a missing file is
# silently empty with status 0; include of a missing file writes a
# diagnostic and the final status is non-zero.
# MUTANT    m4.include_empty
# MUTANT    m4.include_never
# MUTANT    m4.include_no_diag
# MUTANT    m4.include_status0
# MUTANT    m4.include_silent_ok
# MUTANT    m4.include_unscanned
# MUTANT    m4.include_no_final_newline
# MUTANT    m4.sinclude_never
# MUTANT    m4.sinclude_errors
# MUTANT    m4.nodiag_all
# VARIANT   m4.engine
# VARIANT   m4.err_status2
# VARIANT   m4.include_abort
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'included text\n' >inc
printf 'ifelse(a,a,Y,N)\n' >incm
cat >in <<'EOM'
a include(`inc')b
sinclude(`nonexistent')c
sinclude(`inc')dnl
include(`incm')dnl
EOM
cat >in2 <<'EOM'
include(`nonexistent')d
EOM
pc_test
m4 in >out 2>err </dev/null || pc_fail "include/sinclude: nonzero status"
[ "$(cat out)" = 'a included text
b
c
included text
Y' ] || pc_fail "output was '$(cat out)'"
m4 in2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "include of a missing file gave status 0"
[ -s err ] || pc_fail "no diagnostic for the missing include"
pc_pass
