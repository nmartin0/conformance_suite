# An invalid mask fails and leaves the mask unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.008
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.008
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 022
umask 999 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "umask 999 exited $rc"
[ -s err ] || pc_fail "umask 999 wrote no diagnostic"
umask u=q 2>/dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "umask u=q exited $rc"
: >f1 || pc_fail "cannot create a file"
perm f1
[ "$p" = '-rw-r--r--' ] || pc_fail "mask changed: a new file has mode '$p', wanted -rw-r--r--"
pc_pass
