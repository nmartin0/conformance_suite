# kill -NAME works like kill -s NAME.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.008
# MUTANT    kill.dash_name_ignored
# MUTANT    kill.dash_name_rejected
# MUTANT    kill.dash_name_upper_only
# MUTANT    kill.dash_name_lower_only
# MUTANT    kill.dash_name_mixed_rejected
# MUTANT    kill.dash_name_needs_prefix
# MUTANT    kill.send_status1
# MUTANT    kill.send_stdout
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.008
p= q= r= u=
trap 'command kill -s KILL "$p" "$q" "$r" "$u" 2>/dev/null' EXIT
pc_test
# the shell's own kill, then the utility run directly; the name is
# recognized as -s recognizes it: without a prefix, in any letter case
for form in "kill" "env kill"; do
	sleep 30 &
	p=$!
	sleep 30 &
	q=$!
	sleep 30 &
	r=$!
	sleep 30 &
	u=$!
	: >out
	$form -KILL "$p" >>out 2>/dev/null || pc_fail "$form -KILL failed"
	$form -TERM "$q" >>out 2>/dev/null || pc_fail "$form -TERM failed"
	$form -usr1 "$r" >>out 2>/dev/null || pc_fail "$form -usr1 failed"
	$form -Usr2 "$u" >>out 2>/dev/null || pc_fail "$form -Usr2 failed"
	[ ! -s out ] || pc_fail "$form wrote to standard output"
	wait "$p"
	n=$(command kill -l "$?")
	[ "$n" = KILL ] || pc_fail "after $form -KILL the process ended by '$n'"
	wait "$q"
	n=$(command kill -l "$?")
	[ "$n" = TERM ] || pc_fail "after $form -TERM the process ended by '$n'"
	wait "$r"
	n=$(command kill -l "$?")
	[ "$n" = USR1 ] || pc_fail "after $form -usr1 the process ended by '$n'"
	wait "$u"
	n=$(command kill -l "$?")
	[ "$n" = USR2 ] || pc_fail "after $form -Usr2 the process ended by '$n'"
done
pc_pass
