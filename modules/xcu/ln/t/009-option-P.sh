# ln -P makes a hard link to a symbolic link itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.009
#
# Whether the file system can hold a hard link to a symbolic link is
# outside the utility, so a failing ln -P is reported as unresolved
# rather than failed.
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.009
printf 'd\n' >f || pc_fail "cannot create f"
ln -s f sl || pc_fail "cannot create symbolic link"
pc_test
ln -P sl h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_unresolved "ln -P exited with status $rc (the file system may not allow links to symbolic links)"
[ -h h ] || pc_fail "h is not a symbolic link"
pc_pass
