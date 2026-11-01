# chmod with a symbolic mode that names who applies +, - and = to the named classes only, several clauses separated by commas.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.002
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.who_ignored
# MUTANT    chmod.wholist_first_only
# MUTANT    chmod.who_go_swapped
# MUTANT    chmod.a_not_o
# MUTANT    chmod.plus_as_eq
# MUTANT    chmod.minus_noop
# MUTANT    chmod.minus_clears_class
# MUTANT    chmod.eq_as_plus
# MUTANT    chmod.eq_clears_all
# MUTANT    chmod.eq_empty_noop
# MUTANT    chmod.eq_empty_all
# MUTANT    chmod.comma_first_only
# MUTANT    chmod.comma_last_only
# MUTANT    chmod.umask_with_who
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
#
# A restrictive mask is set: a named who must not be limited by it.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.002
# The listing may add a flag after the mode, and a directory may carry the
# set-group-ID bit from its creation; both are left to the implementation.
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
	case $mo in
	d??????[sS]???)
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
mk 654 f1
mk 777 f2 f3 f6 f7
mk 644 f4
mk 640 f5
pc_test
umask 077
run u+x,g-r,o+w f1
want f1 -rwx--xrw- "u+x,g-r,o+w"
run g=r f2
want f2 -rwxr--rwx "g=r"
run u= f3
want f3 ----rwxrwx "u="
run a=,u+rw,g+r f4
want f4 -rw-r----- "a=,u+rw,g+r"
run a+x f5
want f5 -rwxr-x--x "a+x"
run ug-w,o=r f6
want f6 -r-xr-xr-- "ug-w,o=r"
run a-x f7
want f7 -rw-rw-rw- "a-x"
pc_pass
