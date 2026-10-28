# Without -p, creating a directory that already exists is an error with a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.008
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.008
mkdir d || pc_fail "cannot create d"
pc_test
mkdir d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir exited 0 for an existing directory"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
