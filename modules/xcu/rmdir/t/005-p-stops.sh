# rmdir -p removes what it can; a parent that is not empty stays, and the status is non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.005
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.005
mkdir -p a/b/c || pc_fail "cannot create a/b/c"
printf x >a/keep || pc_fail "cannot create a/keep"
pc_test
rmdir -p a/b/c >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rmdir -p exited 0 although a could not be removed"
[ ! -e a/b ] || pc_fail "a/b was not removed"
[ -f a/keep ] || pc_fail "a/keep is gone"
pc_pass
