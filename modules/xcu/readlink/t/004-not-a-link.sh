# For a file that is not a symbolic link, or does not exist, readlink writes a diagnostic, nothing to standard output, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.readlink.004
. "$PC_CORE/pc.sh"
pc_begin XCU.readlink.004
: >f || pc_fail "cannot create f"
pc_test
readlink f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "readlink exited 0 for a regular file"
[ -s err ] || pc_fail "no diagnostic for a regular file"
[ ! -s out ] || pc_fail "output written for a regular file"
readlink nonexistent >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "readlink exited 0 for a missing file"
[ ! -s out ] || pc_fail "output written for a missing file"
pc_pass
