# chown -h changes the owner of a symbolic link itself and not of the file it refers to; without -h the file is changed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.004
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.004
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
ln -s f l || pc_fail "cannot create symbolic link"
ids f || pc_fail "cannot list f"
u0=$iu
pc_test
chown -h 29999 l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "chown -h exited with status $rc (links may not have owners here)"
ids l || pc_fail "cannot list l"
[ "$iu" = 29999 ] || pc_fail "the link has owner $iu, expected 29999"
ids f || pc_fail "cannot list f"
[ "$iu" = "$u0" ] || pc_fail "the file behind the link was changed to $iu"
chown 29998 l >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chown through the link exited with status $rc"
ids f || pc_fail "cannot list f"
[ "$iu" = 29998 ] || pc_fail "without -h the file has owner $iu, expected 29998"
pc_pass
