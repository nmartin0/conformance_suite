# The umask -S output is accepted as the operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.004
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.004
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 027
s=$(umask -S)
[ -n "$s" ] || pc_fail "umask -S wrote nothing"
umask 077
umask "$s" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask '$s' exited $rc"
: >f1 || pc_fail "cannot create a file"
perm f1
[ "$p" = '-rw-r-----' ] || pc_fail "after reinput of '$s' a new file has mode '$p', wanted -rw-r-----"
pc_pass
