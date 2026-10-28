# A symbolic mask is the complement of the permissions given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.005
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.005
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 022
umask u=rwx,g=rx,o= 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask u=rwx,g=rx,o= exited $rc"
: >f1 || pc_fail "cannot create a file"
mkdir d1 || pc_fail "cannot create a directory"
perm f1
[ "$p" = '-rw-r-----' ] || pc_fail "file mode is '$p', wanted -rw-r-----"
perm d1
[ "$p" = 'drwxr-x---' ] || pc_fail "directory mode is '$p', wanted drwxr-x---"
pc_pass
