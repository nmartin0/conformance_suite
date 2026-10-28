# cmp -s writes nothing to standard output or standard error and reports the result in the exit status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.004
printf 'abc\n' >f1 || pc_fail "cannot create f1"
printf 'abd\n' >f2 || pc_fail "cannot create f2"
printf 'abc\n' >f3 || pc_fail "cannot create f3"
pc_test
cmp -s f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -s exited with status $rc for different files"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s wrote output for different files"
cmp -s f1 f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp -s exited with status $rc for identical files"
[ ! -s out ] && [ ! -s err ] || pc_fail "cmp -s wrote output for identical files"
pc_pass
