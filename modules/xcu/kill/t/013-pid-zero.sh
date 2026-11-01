# kill 0 signals the processes of the caller's own process group.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.013
# MUTANT    kill.pid0_drops
# MUTANT    kill.pid0_rejected
# MUTANT    kill.pid0_exit1
#
# Safety: the group is made with job control (a background subshell is a
# job of its own group). Nothing is sent until ps has shown that the group
# holds only the subshell, the sleep it started and that sleep's helpers,
# and that the test's own group is a different one; if that cannot be
# shown the result is UNRESOLVED and no signal is sent.
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.013
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
	rm -f kid go rc st
	(
		set +m
		trap : TERM
		sleep 30 &
		k=$!
		echo "$k" >kid
		i=0
		while [ ! -e go ] && [ "$i" -lt 20 ]; do
			sleep 1
			i=$((i + 1))
		done
		[ -e go ] || exit 3
		$form -s TERM 0 >out 2>/dev/null
		echo $? >rc
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
	: >go
	wait "$L"
	if [ ! -s rc ]; then
		pc_fail "$form -s TERM 0 reported nothing (it did not return)"
	fi
	read -r rcv <rc
	# the utility run directly is itself a member of the group and may be
	# ended by the signal it sends, so only the shell's own status is read
	if [ "$form" = kill ]; then
		[ "$rcv" -eq 0 ] || pc_fail "$form -s TERM 0 exited $rcv"
	fi
	[ -s st ] || pc_fail "the sleep in the group was not reported ended after $form -s TERM 0"
	read -r sst <st
	[ "$sst" -gt 128 ] || pc_fail "the sleep in the group was not ended by a signal after $form -s TERM 0 (status $sst)"
	n=$(command kill -l "$sst")
	[ "$n" = TERM ] || pc_fail "the sleep in the group ended by '$n' after $form -s TERM 0, wanted TERM"
done
pc_pass
