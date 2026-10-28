# unlink on a symbolic link removes the link itself, not the file it points to.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unlink.003
. "$PC_CORE/pc.sh"
pc_begin XCU.unlink.003
printf 'x\n' >f || pc_fail "cannot create f"
ln -s f s || pc_fail "cannot create symbolic link"
pc_test
unlink s >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unlink exited with status $rc"
[ ! -h s ] && [ ! -e s ] || pc_fail "the link still exists"
[ -f f ] || pc_fail "f was removed"
pc_pass
