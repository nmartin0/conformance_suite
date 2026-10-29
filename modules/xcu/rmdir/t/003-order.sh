# A subdirectory named before its parent can be removed together with it, in one invocation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdir.003
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdir.003
mkdir -p a/b || pc_fail "cannot create a/b"
[ -d a/b ] || pc_fail "a/b was not created"
pc_test
rmdir a/b a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rmdir exited with status $rc"
[ ! -e a ] || pc_fail "a still exists"
pc_pass
