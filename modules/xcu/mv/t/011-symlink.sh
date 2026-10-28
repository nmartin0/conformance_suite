# mv moves a symbolic link as a link, not what it refers to.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.011
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.011
printf 'x\n' >f || pc_fail "cannot create f"
ln -s f l || pc_fail "cannot create symbolic link"
pc_test
mv l m >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ ! -h l ] && [ ! -e l ] || pc_fail "l still exists"
[ -h m ] || pc_fail "m is not a symbolic link"
t=$(ls -ld m) || pc_fail "cannot list m"
t=${t##* -> }
[ "$t" = f ] || pc_fail "link contents are '$t', expected f"
[ -f f ] || pc_fail "f was changed"
pc_pass
