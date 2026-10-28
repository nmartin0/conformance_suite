# chown owner:group sets both the user ID and the group ID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.002
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.002
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
pc_test
chown 29999:29998 f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chown exited with status $rc"
ids f || pc_fail "cannot list f"
[ "$iu" = 29999 ] || pc_fail "owner is $iu, expected 29999"
[ "$ig" = 29998 ] || pc_fail "group is $ig, expected 29998"
pc_pass
