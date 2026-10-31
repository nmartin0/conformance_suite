# ln -P makes a hard link to a symbolic link itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.009
#
# ln -P is specified as linkat() with a zero flag, which makes a hard link
# to the symbolic link itself; no error of that function lets a system
# decline. A file system that cannot hold hard links at all is told apart
# by an ordinary hard link made first (unresolved if that fails).
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.009
printf 'd\n' >f || pc_fail "cannot create f"
ln -s f sl || pc_fail "cannot create symbolic link"
ln f ctl >/dev/null 2>&1 </dev/null || pc_unresolved "setup: an ordinary hard link cannot be made here"
pc_test
ln -P sl h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P exited with status $rc"
[ -h h ] || pc_fail "h is not a symbolic link"
pc_pass
