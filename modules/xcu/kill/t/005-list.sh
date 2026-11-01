# kill -l writes the signal names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.005
# MUTANT    kill.list_sig_prefix
# MUTANT    kill.list_lower
# MUTANT    kill.list_no_newline
# MUTANT    kill.list_comma
# MUTANT    kill.list_no_hup
# MUTANT    kill.list_no_kill
# MUTANT    kill.list_no_term
# MUTANT    kill.list_no_tstp
# MUTANT    kill.list_stderr
# MUTANT    kill.list_status1
# MUTANT    kill.list_empty
# VARIANT   kill.list_horizontal
# VARIANT   kill.list_vertical
# VARIANT   kill.list_mixed
# VARIANT   kill.list_extra
# VARIANT   kill.list_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.005
nl='
'
pc_test
# the shell's own kill, then the utility run directly
for form in "kill" "env kill"; do
	$form -l >out 2>/dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$form -l exited $rc"
	pc_need_file out "the output of $form -l"
	content=$(cat out; echo x)
	content=${content%x}
	[ -n "$content" ] || pc_fail "$form -l wrote nothing"
	[ "${content%"$nl"}" != "$content" ] || pc_fail "$form -l: the last name is not followed by a newline"
	names=' '
	for tok in $content; do
		case $tok in
		SIG*) pc_fail "$form -l: name '$tok' has the SIG prefix" ;;
		*[!A-Z0-9+-]*) pc_fail "$form -l: name '$tok' is not upper case letters and digits" ;;
		esac
		names="$names$tok "
	done
	for need in HUP INT QUIT ABRT KILL ALRM TERM USR1 USR2 CHLD CONT STOP TSTP; do
		case $names in
		*" $need "*) ;;
		*) pc_fail "$form -l: $need is not in the list" ;;
		esac
	done
done
pc_pass
