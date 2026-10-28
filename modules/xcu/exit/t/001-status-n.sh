# exit n ends the shell with exit status n for values from 0 to 255.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exit.001
. "$PC_CORE/pc.sh"
pc_begin XCU.exit.001
pc_test
for n in 0 1 2 42 99 125 126 127 128 200 255; do
	${PC_SH:-sh} -c "exit $n" </dev/null
	rc=$?
	[ "$rc" -eq "$n" ] || pc_fail "exit $n gave status $rc"
done
pc_pass
