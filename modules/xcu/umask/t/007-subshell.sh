# umask in a subshell leaves the parent's mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.007
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.007
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 022
( umask 077 )
: >f1 || pc_fail "cannot create a file"
perm f1
[ "$p" = '-rw-r--r--' ] || pc_fail "a file made after the subshell has mode '$p', wanted -rw-r--r--"
pc_pass
