# With -p, a path prefix component that is a file is an error and the file is untouched.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mkdir.010
. "$PC_CORE/pc.sh"
pc_begin XCU.mkdir.010
printf x >f || pc_fail "cannot create f"
pc_test
mkdir -p f/x >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mkdir -p exited 0 through a regular file"
[ -f f ] || pc_fail "f is no longer a regular file"
pc_pass
