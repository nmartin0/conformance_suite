# The output of umask restores the mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.002
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.002
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 027
m=$(umask)
[ -n "$m" ] || pc_fail "umask wrote nothing"
umask 000
umask "$m" 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask '$m' exited $rc"
: >f1 || pc_fail "cannot create a file"
perm f1
[ "$p" = '-rw-r-----' ] || pc_fail "after reinput of '$m' a new file has mode '$p', wanted -rw-r-----"
pc_pass
