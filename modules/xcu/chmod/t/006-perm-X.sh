# The permission X sets execute/search only for directories and for files that already have an execute bit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.006
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.X_always
# MUTANT    chmod.X_never
# MUTANT    chmod.X_dir_not
# MUTANT    chmod.X_owner_only
# MUTANT    chmod.X_ignores_who
# MUTANT    chmod.X_minus_noop
# MUTANT    chmod.X_eq_noop
# MUTANT    chmod.X_first_operand
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.dir_setgid
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.006
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
mk 644 f1
mk 744 f2 f5 f7
mk 001 f3
mk 010 f4
mk 755 f6
mkdir d || pc_fail "cannot create d"
chmod 644 d || pc_fail "cannot chmod d"
pc_test
run a+X f1 f2 d
want f1 -rw-r--r-- "a+X on a file without execute bits"
want f2 -rwxr-xr-x "a+X on a file with owner execute"
want d drwxr-xr-x "a+X on a directory"
run a+X f3 f4
want f3 ---x--x--x "a+X with only other execute"
want f4 ---x--x--x "a+X with only group execute"
run g+X f5
want f5 -rwxr-xr-- "g+X"
run go=X f7
want f7 -rwx--x--x "go=X"
run a-X f6
want f6 -rw-r--r-- "a-X"
pc_pass
