# + clears and - sets bits of the current mask.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.006
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.006
perm() {
	set -- $(ls -ld "$1")
	p=${1%"${1#??????????}"}
}
pc_test
umask 077
umask g+rx 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask g+rx exited $rc"
: >f1 || pc_fail "cannot create a file"
perm f1
[ "$p" = '-rw-r-----' ] || pc_fail "after 077 and g+rx a file has mode '$p', wanted -rw-r-----"
umask 000
umask go-w 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask go-w exited $rc"
: >f2 || pc_fail "cannot create a file"
perm f2
[ "$p" = '-rw-r--r--' ] || pc_fail "after 000 and go-w a file has mode '$p', wanted -rw-r--r--"
pc_pass
