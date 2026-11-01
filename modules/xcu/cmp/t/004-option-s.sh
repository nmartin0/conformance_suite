# cmp -s writes nothing to standard output or standard error and reports the result in the exit status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.004
# MUTANT    cmp.s_ignored
# MUTANT    cmp.s_stdout_msg
# MUTANT    cmp.s_stderr_msg
# MUTANT    cmp.s_ident_err
# MUTANT    cmp.s_differ_exit0
# MUTANT    cmp.s_ident_exit1
# MUTANT    cmp.s_eof_diag
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.004
printf 'abc\n' >f1 || pc_fail "cannot create f1"
printf 'abd\n' >f2 || pc_fail "cannot create f2"
printf 'abc\n' >f3 || pc_fail "cannot create f3"
printf 'abc' >s1 || pc_fail "cannot create s1"
printf 'abcd' >s2 || pc_fail "cannot create s2"
pc_test
cmp -s f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -s exited with status $rc for different files"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s wrote output for different files"
cmp -s f1 f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp -s exited with status $rc for identical files"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s wrote output for identical files"
# one file is the first part of the other: different, and still silent
cmp -s s1 s2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -s exited with status $rc for a file that is a prefix of the other"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s wrote output for a prefix"
cmp -s s2 s1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -s (reversed) exited with status $rc for a prefix"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s (reversed) wrote output for a prefix"
pc_pass
