# cp -R -P copies a symbolic link in the hierarchy as a symbolic link with the same contents.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.014
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.014
mkdir src || pc_fail "cannot create src"
printf 'x\n' >src/f || pc_fail "cannot create src/f"
ln -s f src/l || pc_fail "cannot create symbolic link"
pc_test
cp -R -P src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R -P exited with status $rc"
[ -h dst/l ] || pc_fail "dst/l is not a symbolic link"
t=$(ls -ld dst/l) || pc_fail "cannot list dst/l"
t=${t##* -> }
[ "$t" = f ] || pc_fail "link contents are '$t', expected f"
pc_pass
