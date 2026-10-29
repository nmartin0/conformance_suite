# kill -l with a signal number names the signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.007
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.007
pc_test
for pair in 1:HUP 2:INT 3:QUIT 6:ABRT 9:KILL 14:ALRM 15:TERM; do
	num=${pair%%:*}
	want=${pair#*:}
	got=$(kill -l "$num" 2>/dev/null)
	[ "$got" = "$want" ] || pc_fail "kill -l $num wrote '$got', wanted $want"
	got=$(env kill -l "$num" 2>/dev/null)
	[ "$got" = "$want" ] || pc_fail "env kill -l $num wrote '$got', wanted $want"
done
pc_pass
