# kill signals every pid operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.012
# MUTANT    kill.only_first
# MUTANT    kill.only_last
# MUTANT    kill.send_status1
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.012
p= q= r=
trap 'command kill -s KILL "$p" "$q" "$r" 2>/dev/null' EXIT
pc_test
# the shell's own kill, then the utility run directly
for form in "kill" "env kill"; do
	sleep 30 &
	p=$!
	sleep 30 &
	q=$!
	sleep 30 &
	r=$!
	sleep 1
	$form -s USR1 "$p" "$q" "$r" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$form with three live processes exited $rc"
	[ ! -s out ] || pc_fail "$form wrote to standard output"
	for x in "$p" "$q" "$r"; do
		wait "$x"
		n=$(command kill -l "$?")
		[ "$n" = USR1 ] || pc_fail "a process named by $form -s USR1 with three operands ended by '$n'"
	done
done
pc_pass
