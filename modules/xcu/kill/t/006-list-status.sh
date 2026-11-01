# kill -l with a wait status names the signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.006
# MUTANT    kill.lop_prefix
# MUTANT    kill.lop_lower
# MUTANT    kill.lop_no_newline
# MUTANT    kill.lop_stderr
# MUTANT    kill.lop_all
# MUTANT    kill.lop_status1
# MUTANT    kill.lop_off1
# MUTANT    kill.lop_swap
# MUTANT    kill.lop_status_unsupported
# MUTANT    kill.lop_blank_line
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.006
p= q=
trap 'command kill -s KILL "$p" "$q" 2>/dev/null' EXIT
nl='
'
pc_test
# the shell's own kill (the exit status of a signalled process is a
# shell notion; the utility run alone is not asked to read it)
for form in "kill"; do
	sleep 30 &
	p=$!
	kill -s KILL "$p" || pc_fail "kill failed"
	wait "$p"
	st=$?
	$form -l "$st" >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$form -l $st exited $rc"
	pc_need_file out "the output of $form -l $st"
	got=$(cat out; echo x)
	[ "$got" = "KILL${nl}x" ] || pc_fail "$form -l $st wrote something other than KILL and a newline"
	for sig in TERM USR1 USR2; do
		sleep 30 &
		q=$!
		sleep 1
		kill -s "$sig" "$q" || pc_fail "kill failed"
		wait "$q"
		st=$?
		$form -l "$st" >out 2>/dev/null
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "$form -l $st exited $rc"
		pc_need_file out "the output of $form -l $st"
		got=$(cat out; echo x)
		[ "$got" = "$sig${nl}x" ] || pc_fail "$form -l $st (a process ended by $sig) wrote something other than $sig and a newline"
	done
done
pc_pass
