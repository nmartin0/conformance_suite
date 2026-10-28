# An owner operand that is no user name and not numeric is an error and changes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.008
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.008
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
ids f || pc_fail "cannot list f"
u0=$iu
g0=$ig
pc_test
chown pc_no_such_user_9z8y7 f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chown exited 0 for an unknown user name"
[ -s err ] || pc_fail "no diagnostic"
ids f || pc_fail "cannot list f"
[ "$iu" = "$u0" ] && [ "$ig" = "$g0" ] || pc_fail "the file was changed"
pc_pass
