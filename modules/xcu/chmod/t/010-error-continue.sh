# If one file cannot be changed, chmod diagnoses it, changes the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.010
# MUTANT    chmod.err_no_diag
# MUTANT    chmod.err_status0
# MUTANT    chmod.err_abort
# MUTANT    chmod.err_skips_next
# MUTANT    chmod.err_status_last
# MUTANT    chmod.err_status_first
# MUTANT    chmod.err_diag_stdout
# MUTANT    chmod.err_diag_both
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.status2
# VARIANT   chmod.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.010
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

for n in f g; do
	: >"$n" || pc_fail "cannot create $n"
	chmod 644 "$n" || pc_fail "cannot chmod $n"
done
[ ! -e nonexistent ] && [ ! -e nonexistent2 ] || pc_fail "the missing names exist"
pc_test
chmod 600 nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chmod exited 0 although a file does not exist"
[ -s err ] || pc_fail "no diagnostic"
[ ! -s out ] || pc_fail "text went to standard output"
want f -rw------- "operand after the failing one"
chmod 600 g nonexistent2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chmod exited 0 although the last file does not exist"
[ -s err ] || pc_fail "no diagnostic for the last operand"
[ ! -s out ] || pc_fail "text went to standard output"
want g -rw------- "operand before the failing one"
pc_pass
