# sed exits with a status above zero for a script it cannot use and for a file it cannot read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.015
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.015
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'x\n' >in || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(sed 'k' in 2>/dev/null </dev/null)
rc=$?
[ "$rc" -gt 0 ] || pc_fail "an unknown command gave status 0"
[ -z "$out" ] || pc_fail "output was written for an invalid script"
sed 's/a/b' in >/dev/null 2>&1 </dev/null
[ $? -gt 0 ] || pc_fail "an unterminated s command gave status 0"
sed -n p nosuchfile >/dev/null 2>&1 </dev/null
[ $? -gt 0 ] || pc_fail "a missing input file gave status 0"
sed p >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "empty standard input gave a nonzero status"
sed -f nosuchfile in >/dev/null 2>&1 </dev/null
[ $? -gt 0 ] || pc_fail "a missing script file gave status 0"
sed -n p in >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "a successful run did not give status 0"
pc_pass
