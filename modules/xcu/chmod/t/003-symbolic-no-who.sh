# Without who, + - and = affect the classes except the bits set in the file creation mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.003
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.umask_ignored
# MUTANT    chmod.umask_inverted
# MUTANT    chmod.umask_not_plus
# MUTANT    chmod.umask_not_minus
# MUTANT    chmod.umask_not_eq
# MUTANT    chmod.eq_masked_clear
# MUTANT    chmod.eq_nowho_keeps
# MUTANT    chmod.nowho_as_user
# MUTANT    chmod.umask_user_part_ignored
# MUTANT    chmod.umask_group_part_ignored
# MUTANT    chmod.umask_other_part_ignored
# MUTANT    chmod.umask_with_who
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.strict_opts
#
# A mode that begins with a hyphen is given after the end-of-options marker, as
# the utility's usage notes show: the syntax guidelines make a bare -w an
# unknown option, which strict_opts models.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.003
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

mk() { # mode file...
	m=$1
	shift
	for n; do
		: >"$n" || pc_fail "cannot create $n"
		chmod "$m" "$n" || pc_fail "cannot chmod $n"
	done
}
mk 666 f1
mk 000 f2 f5
mk 777 f3
mk 644 f4 f6
pc_test
umask 022
run -- -w f1
want f1 -r--rw-rw- "-w under mask 022"
run +w f2
want f2 --w------- "+w under mask 022"
run =rw f3
want f3 -rw-r--r-- "=rw under mask 022"
umask 011
run +x f4
want f4 -rwxr--r-- "+x under mask 011"
umask 0750
run +rwx f5
want f5 -----w-rwx "+rwx under mask 0750"
umask 077
run g+w f6
want f6 -rw-rw-r-- "g+w under mask 077 (who given)"
pc_pass
