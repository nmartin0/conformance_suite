# kill -- -pgid signals the processes of that group.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.014
# MUTANT    kill.neg_as_positive
# MUTANT    kill.neg_drops
# MUTANT    kill.neg_rejected
# MUTANT    kill.dd_rejected
#
# Safety: the group is made with job control (a background subshell is a
# job of its own group). Nothing is sent until ps has shown that the group
# holds only the subshell, the sleep it started and that sleep's helpers,
# and that the test's own group is a different one; if that cannot be
# shown the result is UNRESOLVED and no signal is sent.
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.014
L= K=
trap 'command kill -s KILL "$L" "$K" 2>/dev/null' EXIT
set -m 2>/dev/null
case $- in *m*) ;; *) pc_unresolved "job control cannot be turned on" ;; esac
command -v ps >/dev/null 2>&1 || pc_unresolved "ps is not available to check process groups"
mypg=$(ps -o pgid= -p $$ 2>/dev/null | tr -d ' ')
case $mypg in
''|*[!0-9]*) pc_unresolved "ps cannot report a process group" ;;
esac
pc_test
for form in "kill" "env kill"; do
	rm -f kid st
	(
		set +m
		trap : TERM
		sleep 30 &
		k=$!
		echo "$k" >kid
		wait "$k"
		wait "$k"
		echo $? >st
	) &
	L=$!
	i=0
	while [ ! -s kid ] && [ "$i" -lt 20 ]; do
		sleep 1
		i=$((i + 1))
	done
	[ -s kid ] || pc_unresolved "the group leader did not start its sleep"
	read -r K <kid
	lpg=$(ps -o pgid= -p "$L" 2>/dev/null | tr -d ' ')
	kpg=$(ps -o pgid= -p "$K" 2>/dev/null | tr -d ' ')
	[ "$lpg" = "$L" ] && [ "$kpg" = "$L" ] && [ "$lpg" != "$mypg" ] || pc_unresolved "the subshell does not lead a group of its own with the sleep in it"
	# every member of the group is the leader, the sleep, or a child of the leader
	ps -A -o pid= -o ppid= -o pgid= >members 2>/dev/null || pc_unresolved "ps cannot list the processes"
	while read -r mp mpp mg; do
		[ "$mg" = "$L" ] || continue
		[ "$mp" = "$L" ] || [ "$mp" = "$K" ] || [ "$mpp" = "$L" ] || pc_unresolved "the group holds a process the test did not start"
	done <members
	# the group is signalled by its negated ID, after the "--" that keeps
	# the operand from being read as an option
	$form -s TERM -- "-$L" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$form -s TERM -- -$L exited $rc"
	[ ! -s out ] || pc_fail "$form wrote to standard output"
	wait "$L"
	[ -s st ] || pc_fail "the sleep in the group was not reported ended after $form -s TERM -- -$L"
	read -r sst <st
	[ "$sst" -gt 128 ] || pc_fail "the sleep in the group was not ended by a signal after $form -s TERM -- -$L (status $sst)"
	n=$(command kill -l "$sst")
	[ "$n" = TERM ] || pc_fail "the sleep in the group ended by '$n' after $form -s TERM -- -$L, wanted TERM"
done
pc_pass
