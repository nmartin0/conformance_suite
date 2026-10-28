# cp -R -L -P: the last of -H/-L/-P wins; the link is copied as a link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.024
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.024
mkdir src || pc_unresolved "cannot create src"
printf 'x\n' >src/f || pc_unresolved "cannot create src/f"
ln -s f src/l || pc_unresolved "cannot create symbolic link"
pc_test
cp -R -L -P src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
[ -h dst/l ] || pc_fail "dst/l is not a symbolic link"
pc_pass
