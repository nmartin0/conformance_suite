# For identical files cmp writes nothing and exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.001
printf 'abc\ndef\n' >f1 || pc_fail "cannot create f1"
printf 'abc\ndef\n' >f2 || pc_fail "cannot create f2"
: >e1 || pc_fail "cannot create e1"
: >e2 || pc_fail "cannot create e2"
pc_test
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for identical files"
[ ! -s out ] || pc_fail "output written"
[ ! -s err ] || pc_fail "diagnostic written"
cmp e1 e2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for empty files"
[ ! -s out ] && [ ! -s err ] || pc_fail "output written for empty files"
pc_pass
