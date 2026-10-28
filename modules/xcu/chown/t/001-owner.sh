# chown sets the user ID of the file to the numeric owner operand and leaves the group alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.001
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.001
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

mkdir w || pc_fail "cannot create w"
: >w/f || pc_fail "cannot create w/f"
ids w/f || pc_fail "cannot list w/f"
g0=$ig
pc_test
chown 29999 w/f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chown exited with status $rc"
ids w/f || pc_fail "cannot list w/f"
[ "$iu" = 29999 ] || pc_fail "owner is $iu, expected 29999"
[ "$ig" = "$g0" ] || pc_fail "group changed from $g0 to $ig"
pc_pass
