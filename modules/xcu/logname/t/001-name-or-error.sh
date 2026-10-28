# logname gives a name, or an error and no output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.logname.001
. "$PC_CORE/pc.sh"
pc_begin XCU.logname.001
pc_test
logname </dev/null >out 2>err
rc=$?
if [ "$rc" -eq 0 ]; then
	n=0
	while IFS= read -r line; do
		n=$((n + 1))
		first=${first-$line}
	done <out
	[ "$n" -eq 1 ] || pc_fail "logname exited zero but wrote $n lines"
	[ -n "$first" ] || pc_fail "logname exited zero but wrote an empty name"
else
	[ ! -s out ] || pc_fail "logname exited $rc but wrote to standard output: $(cat out)"
	[ -s err ] || pc_fail "logname exited $rc but wrote no diagnostic message"
fi
(exec logname) </dev/null >out 2>err
rc2=$?
if [ "$rc2" -eq 0 ]; then
	[ "$rc" -eq 0 ] || pc_fail "exec logname succeeded where logname failed"
	[ -s out ] || pc_fail "exec logname exited zero but wrote nothing"
else
	[ ! -s out ] || pc_fail "exec logname exited $rc2 but wrote to standard output"
	[ -s err ] || pc_fail "exec logname exited $rc2 but wrote no diagnostic message"
fi
pc_pass
