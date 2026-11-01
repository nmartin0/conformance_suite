# kill -number sends the signal for 0, 9 and 15.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.009
# MUTANT    kill.dash_num_rejected
# MUTANT    kill.dash_num_9_term
# MUTANT    kill.dash_num_15_kill
# MUTANT    kill.dash_num_zero_term
# MUTANT    kill.dash_num_14_term
# MUTANT    kill.send_status1
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.009
p= q= u=
trap 'command kill -s KILL "$p" "$q" "$u" 2>/dev/null' EXIT
pc_test
# the shell's own kill, then the utility run directly. Number 14
# (which ends an ordinary process) joins 0, 9 and 15 from the page's table.
for form in "kill" "env kill"; do
	sleep 30 &
	p=$!
	sleep 30 &
	q=$!
	sleep 30 &
	u=$!
	$form -0 "$p" 2>/dev/null || pc_fail "$form -0 failed for a live process"
	$form -9 "$p" 2>/dev/null || pc_fail "$form -9 failed"
	$form -15 "$q" 2>/dev/null || pc_fail "$form -15 failed"
	$form -14 "$u" 2>/dev/null || pc_fail "$form -14 failed"
	wait "$p"
	n=$(command kill -l "$?")
	[ "$n" = KILL ] || pc_fail "after $form -9 the process ended by '$n'"
	wait "$q"
	n=$(command kill -l "$?")
	[ "$n" = TERM ] || pc_fail "after $form -15 the process ended by '$n'"
	wait "$u"
	n=$(command kill -l "$?")
	[ "$n" = ALRM ] || pc_fail "after $form -14 the process ended by '$n'"
done
pc_pass
