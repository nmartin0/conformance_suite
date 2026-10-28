# An octal mask sets the permissions of later files and directories.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.001
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.001
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 027 >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask 027 exited $rc"
[ ! -s out ] || pc_fail "umask with an operand wrote to standard output"
: >f1 || pc_fail "cannot create a file"
mkdir d1 || pc_fail "cannot create a directory"
perm f1
[ "$p" = '-rw-r-----' ] || pc_fail "file mode is '$p', wanted -rw-r-----"
perm d1
[ "$p" = 'drwxr-x---' ] || pc_fail "directory mode is '$p', wanted drwxr-x---"
pc_pass
