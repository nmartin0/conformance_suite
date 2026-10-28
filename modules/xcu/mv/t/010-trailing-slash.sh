# mv of a non-directory to a target_file with a trailing slash is an error and the source is not moved.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.010
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.010
printf 'x\n' >a || pc_fail "cannot create a"
pc_test
mv a b/ >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mv exited 0 for a target with a trailing slash"
[ -f a ] || pc_fail "the source was moved"
[ ! -e b ] || pc_fail "b was created"
pc_pass
