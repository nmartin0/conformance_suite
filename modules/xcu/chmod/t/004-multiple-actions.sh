# One clause may carry several actions, applied in order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chmod.004
# MUTANT    chmod.ok_status1
# MUTANT    chmod.stdout_noise
# MUTANT    chmod.actions_reverse
# MUTANT    chmod.actions_first_only
# MUTANT    chmod.actions_last_only
# MUTANT    chmod.action_who_lost
# VARIANT   chmod.reimpl
# VARIANT   chmod.ls_flag
#
# The mask is cleared so that an action that loses its who list (and so
# acts on every class) shows.
. "$PC_CORE/pc.sh"
pc_begin XCU.chmod.004
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

: >f || pc_fail "cannot create f"
: >f2 || pc_fail "cannot create f2"
chmod 066 f || pc_fail "cannot chmod f"
chmod 677 f2 || pc_fail "cannot chmod f2"
pc_test
umask 0
run u+rwx-w f
want f -r-xrw-rw- "u+rwx-w"
run g=o-w f2
want f2 -rw-r-xrwx "g=o-w"
pc_pass
