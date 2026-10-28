# cp -R -H follows a symbolic link given as an operand but copies links found in the hierarchy as links.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.016
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.016
mkdir real || pc_fail "cannot create real"
printf 'x\n' >real/f || pc_fail "cannot create real/f"
ln -s f real/inner || pc_fail "cannot create inner link"
ln -s real top || pc_fail "cannot create top link"
pc_test
cp -R -H top dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R -H exited with status $rc"
[ ! -h dst ] || pc_fail "dst is a symbolic link"
[ -d dst ] || pc_fail "dst is not a directory"
[ -f dst/f ] || pc_fail "dst/f is missing"
[ -h dst/inner ] || pc_fail "dst/inner is not a symbolic link"
pc_pass
