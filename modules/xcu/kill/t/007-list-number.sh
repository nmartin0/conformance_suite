# kill -l with a signal number names the signal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.007
# MUTANT    kill.lop_prefix
# MUTANT    kill.lop_lower
# MUTANT    kill.lop_no_newline
# MUTANT    kill.lop_stderr
# MUTANT    kill.lop_all
# MUTANT    kill.lop_status1
# MUTANT    kill.lop_off1
# MUTANT    kill.lop_swap
# MUTANT    kill.lop_number_unsupported
# MUTANT    kill.lop_blank_line
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.007
nl='
'
pc_test
for pair in 1:HUP 2:INT 3:QUIT 6:ABRT 9:KILL 14:ALRM 15:TERM; do
	num=${pair%%:*}
	want=${pair#*:}
	for form in "kill" "env kill"; do
		$form -l "$num" >out 2>/dev/null
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "$form -l $num exited $rc"
		pc_need_file out "the output of $form -l $num"
		got=$(cat out; echo x)
		[ "$got" = "$want${nl}x" ] || pc_fail "$form -l $num did not write exactly $want and a newline"
	done
done
pc_pass
