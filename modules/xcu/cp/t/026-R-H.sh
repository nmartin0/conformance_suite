# cp -R -H follows a symbolic link operand but not links found in the hierarchy.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.026
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.026
mkdir real || pc_unresolved "cannot create real"
printf 'x\n' >real/f || pc_unresolved "cannot create real/f"
ln -s f real/inner || pc_unresolved "cannot create symbolic link"
ln -s real top || pc_unresolved "cannot create symbolic link"
pc_test
cp -R -H top dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
[ -d dst ] && [ ! -h dst ] || pc_fail "dst is not a directory"
[ -f dst/f ] || pc_fail "dst/f missing"
[ -h dst/inner ] || pc_fail "dst/inner is not a symbolic link"
pc_pass
