# tail -f with no file operand and standard input a pipe or FIFO ignores -f: tail ends once the input ends.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.008
# MUTANT    tail.f_pipe_follows
# MUTANT    tail.f_pipe_nothing
# MUTANT    tail.f_pipe_status1
# VARIANT   tail.byte_writes
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.008
LC_ALL=POSIX
export LC_ALL
NL='
'
mkfifo pipe || pc_unresolved "cannot create a FIFO"
trap 'kill $(cat tpid 2>/dev/null) 2>/dev/null' EXIT
pc_test
(
	tail -f -n 2 >out 2>/dev/null <pipe &
	echo $! >tpid
	wait $!
	echo $? >exited
) &
printf '1\n2\n3\n' >pipe || pc_unresolved "cannot write to the FIFO"
# With -f ignored, tail has copied the input to its end and ends. No time is
# stated, but a tail that is still running after thirty seconds of end of
# input is not ignoring -f.
i=0
while [ ! -e exited ] && [ $i -lt 30 ]; do
	sleep 1
	i=$((i + 1))
done
[ -e exited ] || pc_fail "tail -f did not end after the end of its standard input (-f was not ignored)"
got=$(cat out; printf x)
[ "$got" = "2${NL}3${NL}x" ] || pc_fail "tail -f on standard input did not write the last two lines"
rc=$(cat exited)
[ "$rc" = 0 ] || pc_fail "tail -f on standard input ended with status $rc"
pc_pass
