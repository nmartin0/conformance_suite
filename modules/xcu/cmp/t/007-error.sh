# When a file cannot be read cmp exits with a status above 1, writes a diagnostic (unless -s), and writes nothing to standard output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.007
printf 'abc\n' >f1 || pc_fail "cannot create f1"
pc_test
cmp f1 nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "cmp exited with status $rc for a missing file"
[ ! -s out ] || pc_fail "output written"
[ -s err ] || pc_fail "no diagnostic"
cmp -s f1 nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -gt 1 ] || pc_fail "cmp -s exited with status $rc for a missing file"
[ ! -s out ] || pc_fail "output written by cmp -s"
pc_pass
