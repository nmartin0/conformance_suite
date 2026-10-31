# tail -f on a regular file keeps running and copies data appended to the file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.007
# MUTANT    tail.f_ignored
# MUTANT    tail.f_initial_default
# MUTANT    tail.f_garbled
# MUTANT    tail.f_recopy
# MUTANT    tail.f_skip_byte
# MUTANT    tail.f_twice
# VARIANT   tail.slow_poll
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >grow || pc_fail "cannot create input"
exp1="2${NL}3${NL}"
exp2="2${NL}3${NL}4${NL}5${NL}"
trap 'kill $(cat tpid 2>/dev/null) 2>/dev/null' EXIT
# wait_for DATA: wait up to thirty seconds for the output to be exactly DATA.
# Returns 1 on timeout with the output a prefix of DATA (it may be slow, and no
# time bound is stated); fails when the output is not a prefix of DATA (wrong
# data) or when tail has ended.
wait_for() {
	i=0
	while [ $i -lt 30 ]; do
		got=$(cat out; printf x)
		got=${got%x}
		[ "$got" = "$1" ] && return 0
		case $1 in
		"$got"*) ;;
		*) pc_fail "tail -f wrote the wrong data" ;;
		esac
		[ ! -e exited ] || pc_fail "tail -f ended before all the data was copied"
		sleep 1
		i=$((i + 1))
	done
	return 1
}
pc_test
# The subshell records when tail ends, so that the test need not rely on a
# signal probe that sees a process not yet reaped.
(
	tail -f -n 2 grow >out 2>/dev/null </dev/null &
	echo $! >tpid
	wait $!
	echo $? >exited
) &
i=0
while [ ! -s tpid ] && [ $i -lt 10 ]; do
	sleep 1
	i=$((i + 1))
done
[ -s tpid ] || pc_unresolved "tail did not start"
wait_for "$exp1" || pc_unresolved "tail -f wrote no complete initial output within 30 seconds"
# A tail that is going to end after copying has done so by now.
sleep 3
[ ! -e exited ] || pc_fail "tail -f terminated after copying the last lines"
printf '4\n5\n' >>grow || pc_unresolved "cannot append to the file"
wait_for "$exp2" || pc_unresolved "appended data not copied within 30 seconds (no time bound is stated)"
[ ! -e exited ] || pc_fail "tail -f terminated after copying the appended data"
pc_pass
