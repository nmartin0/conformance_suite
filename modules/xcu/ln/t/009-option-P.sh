# ln -P makes a hard link to a symbolic link itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.009
# MUTANT    ln.P_as_L
# MUTANT    ln.P_new_symlink
# MUTANT    ln.P_dir_wrong_name
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
#
# ln -P is specified as linkat() with a zero flag, which makes a hard link
# to the symbolic link itself; no error of that function lets a system
# decline. A file system that cannot hold hard links at all is told apart
# by an ordinary hard link made first (unresolved if that fails).
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.009
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

printf 'd\n' >lf9 || pc_fail "cannot create lf9"
ln -s lf9 sl || pc_fail "cannot create symbolic link"
mkdir d || pc_fail "cannot create d"
ln lf9 ctl >/dev/null 2>&1 </dev/null || pc_unresolved "setup: an ordinary hard link cannot be made here"
pc_test
ln -P sl h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P exited with status $rc"
[ ! -s out ] || pc_fail "ln -P wrote to standard output"
[ -h h ] || pc_fail "h is not a symbolic link"
linkof h || pc_fail "cannot list h"
[ "$lk" = lf9 ] || pc_fail "h contains '$lk', expected lf9"
set -- $(ls -ld sl)
[ "$2" = 2 ] || pc_fail "the link count of sl is $2, expected 2 (h is not a second name of the same symbolic link)"
ln -P sl d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P into a directory exited with status $rc"
[ -h d/sl ] || pc_fail "d/sl is not a symbolic link"
pc_pass
