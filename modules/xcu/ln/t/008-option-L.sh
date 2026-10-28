# ln -L makes a hard link to the file a symbolic link refers to.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.008
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.008
printf 'd\n' >f || pc_fail "cannot create f"
ln -s f sl || pc_fail "cannot create symbolic link"
pc_test
ln -L sl h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L exited with status $rc"
[ ! -h h ] || pc_fail "h is a symbolic link"
[ h -ef f ] || pc_fail "h is not a link to f"
pc_pass
