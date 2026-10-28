# mv of a directory onto an existing non-directory is an error that changes neither.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.009
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.009
mkdir d || pc_fail "cannot create d"
printf 'f\n' >f || pc_fail "cannot create f"
pc_test
mv d f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "mv exited 0 for a directory onto a regular file"
[ -d d ] || pc_fail "d is gone"
[ -f f ] || pc_fail "f is no longer a regular file"
pc_pass
