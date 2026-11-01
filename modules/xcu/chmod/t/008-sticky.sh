# On a directory, a+t or t alone sets the sticky bit, a-t clears it, octal 1777 sets it, and an octal mode without it clears it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.008
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.t_ignored
# MUTANT    chmod.t_nowho_ignored
# MUTANT    chmod.t_a_ignored
# MUTANT    chmod.t_minus_noop
# MUTANT    chmod.octal_sticky_dropped
# MUTANT    chmod.octal_sticky_kept
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
# VARIANT   chmod.dir_setgid
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.008
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

mkdir d1 d2 d3 d4 || pc_fail "cannot create directories"
chmod 777 d1 d2 d3 || pc_fail "cannot chmod"
chmod 1777 d4 || pc_fail "cannot chmod d4"
pc_test
run a+t d1
want d1 drwxrwxrwt "a+t"
run +t d2
want d2 drwxrwxrwt "+t"
run 1777 d3
want d3 drwxrwxrwt "1777"
run 777 d3
want d3 drwxrwxrwx "777 after 1777"
run a-t d4
want d4 drwxrwxrwx "a-t"
pc_pass
