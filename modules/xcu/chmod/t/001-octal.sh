# chmod with an octal mode sets the permission bits absolutely, for each file operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.001
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.octal_or
# MUTANT    chmod.octal_clear_only
# MUTANT    chmod.octal_umask
# MUTANT    chmod.octal_decimal
# MUTANT    chmod.octal_go_swapped
# MUTANT    chmod.octal_leading_zero_err
# MUTANT    chmod.octal_ignored
# MUTANT    chmod.operand_first_only
# MUTANT    chmod.operand_last_only
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
#
# The file creation mask is set so that a mode that wrongly honours it shows.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.001
# The listing may add a flag after the mode, and a directory may carry the
# set-group-ID bit from its creation; both are left to the implementation.
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
	case $mo in
	d?????[sS]???)
		mo_a=${mo%"${mo#??????}"}
		mo_r=${mo#???????}
		mo_c=${mo%"$mo_r"}
		mo_c=${mo_c#??????}
		case $mo_c in s) mo_c=x ;; S) mo_c=- ;; esac
		mo=$mo_a$mo_c$mo_r
		;;
	esac
}
# run ARGS: chmod must succeed and write nothing to standard output.
run() {
	chmod "$@" >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "chmod $* exited with status $rc"
	[ ! -s out ] || pc_fail "chmod $* wrote to standard output"
}
# want FILE MODE LABEL
want() {
	modeof "$1" || pc_fail "cannot list $1"
	[ "$mo" = "$2" ] || pc_fail "$3: $1 has mode $mo, expected $2"
}

for n in f g h; do
	: >"$n" || pc_fail "cannot create $n"
	chmod 777 "$n" || pc_fail "cannot chmod $n"
done
pc_test
umask 022
run 640 f
want f -rw-r----- "640"
run 0600 f g h
want f -rw------- "0600 on three operands"
want g -rw------- "0600 on three operands"
want h -rw------- "0600 on three operands"
run 666 h
want h -rw-rw-rw- "666 under a mask"
pc_pass
