# cksum exits nonzero and writes no result line for a file it cannot read, and 0 when all files were processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cksum.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cksum.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc' >f1 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(cksum nosuchfile 2>/dev/null </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a missing file"
[ -z "$out" ] || pc_fail "a result line was written for a missing file"
err=$(cksum nosuchfile 2>&1 >/dev/null </dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file"
cksum f1 >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "status was not 0 for a readable file"
cksum nosuchfile f1 >/dev/null 2>&1 </dev/null
[ $? -ne 0 ] || pc_fail "status 0 when one of two files was missing"
pc_pass
