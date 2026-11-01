# On a regular file with an execute bit, u+s and g+s set and u-s and g-s clear the set-user-ID and set-group-ID bits, octal 4755 and 2755 set them and 755 clears them, and o+s changes nothing and is not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.007
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.s_ignored
# MUTANT    chmod.s_minus_noop
# MUTANT    chmod.u_s_as_gid
# MUTANT    chmod.u_s_both
# MUTANT    chmod.g_s_as_uid
# MUTANT    chmod.g_s_both
# MUTANT    chmod.g_minus_s_noop
# MUTANT    chmod.u_minus_s_both
# MUTANT    chmod.a_s_only_u
# MUTANT    chmod.o_s_sets
# MUTANT    chmod.o_s_clears
# MUTANT    chmod.o_s_error
# MUTANT    chmod.octal_special_dropped
# MUTANT    chmod.octal_setgid_dropped
# MUTANT    chmod.octal_special_kept
# MUTANT    chmod.octal_setgid_kept
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.xclear_clears_s
# VARIANT   chmod.s_ignored_noexec
#
# The set-group-ID cases need a system and a user for which the bit can be
# held on a file at all (an unprivileged process may be refused it); that is
# probed with the real utility before the test phase, else UNRESOLVED.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.007
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
mk 755 f1 f2 f4 f6
mk 4755 f3
mk 6755 f5
modeof f5 || pc_fail "cannot list f5"
[ "$mo" = -rwsr-sr-x ] || pc_unresolved "set-user-ID and set-group-ID cannot both be held on a file here"
pc_test
run u+s f1
want f1 -rwsr-xr-x "u+s"
run u-s f1
want f1 -rwxr-xr-x "u-s"
run g+s f1
want f1 -rwxr-sr-x "g+s"
run g-s f1
want f1 -rwxr-xr-x "g-s"
run 4755 f2
want f2 -rwsr-xr-x "4755"
run 755 f2
want f2 -rwxr-xr-x "755 after 4755"
run 2755 f2
want f2 -rwxr-sr-x "2755"
run 755 f2
want f2 -rwxr-xr-x "755 after 2755"
run o+s f3
want f3 -rwsr-xr-x "o+s on a set-user-ID file"
run o+s f4
want f4 -rwxr-xr-x "o+s on a plain file"
run a+s f6
want f6 -rwsr-sr-x "a+s"
run u-s f5
want f5 -rwxr-sr-x "u-s keeps set-group-ID"
run g-s f5
want f5 -rwxr-xr-x "g-s"
pc_pass
