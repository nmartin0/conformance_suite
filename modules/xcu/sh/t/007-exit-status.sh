# sh ends with the status of the last command it ran (as exit with no
# operand does) or with the status given to exit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.007
pc_test
for pair in "true:0" "false:1" "(exit 3):3" "exit 5:5" "(exit 200):200"; do
	cmd=${pair%:*}
	want=${pair##*:}
	${PC_SH:-sh} -c "$cmd" </dev/null
	rc=$?
	[ "$rc" -eq "$want" ] || pc_fail "'$cmd' gave status $rc, expected $want"
done
pc_pass
