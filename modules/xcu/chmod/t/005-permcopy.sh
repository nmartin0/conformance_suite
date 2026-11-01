# The permission letters u, g and o after an operator copy the current permissions of that class.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.005
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.copy_wrong_class
# MUTANT    chmod.copy_plus_noop
# MUTANT    chmod.copy_minus_noop
# MUTANT    chmod.copy_original
# MUTANT    chmod.eq_as_plus
# MUTANT    chmod.plus_as_eq
# MUTANT    chmod.minus_noop
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.005
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
mk 750 f1 f8
mk 640 f2
mk 700 f3
mk 764 f4
mk 675 f5
mk 160 f6
pc_test
run g=u f1
want f1 -rwxrwx--- "g=u"
run o=g f2
want f2 -rw-r--r-- "o=g"
run go=u f3
want f3 -rwxrwxrwx "go=u"
run g=o f4
want f4 -rwxr--r-- "g=o (replaces)"
run g-o f5
want f5 -rw--w-r-x "g-o"
run g+u f6
want f6 ---xrwx--- "g+u (keeps own bits)"
run u=g,g=u f8
want f8 -r-xr-x--- "u=g,g=u (second clause sees the first)"
pc_pass
