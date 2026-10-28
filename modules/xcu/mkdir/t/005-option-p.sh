# mkdir -p creates all missing intermediate directories, for several operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.005
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.005
pc_test
mkdir -p a/b/c x y/z >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mkdir -p exited with status $rc"
[ -d a/b/c ] || pc_fail "a/b/c was not created"
[ -d x ] && [ -d y/z ] || pc_fail "x or y/z was not created"
pc_pass
