# A non-interactive sh that finds a syntax, redirection or variable
# assignment error exits with a status from 1 to 125.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.009
pc_test
i=0
for cmd in 'echo (' ': <pc.nosuchfile' 'readonly PCR=1; PCR=2' 'echo ${PCU1?msg}'; do
	i=$((i + 1))
	${PC_SH:-sh} -c "$cmd" </dev/null 2>/dev/null
	rc=$?
	[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "case $i ($cmd): status $rc, expected 1 to 125"
done
pc_pass
