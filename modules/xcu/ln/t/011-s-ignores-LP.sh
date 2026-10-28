# With -s, the options -L and -P are silently ignored and a symbolic link is made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.011
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.011
printf 'd\n' >f || pc_fail "cannot create f"
pc_test
ln -s -L f s1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s -L exited with status $rc"
[ -h s1 ] || pc_fail "s1 is not a symbolic link"
ln -s -P f s2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s -P exited with status $rc"
[ -h s2 ] || pc_fail "s2 is not a symbolic link"
pc_pass
