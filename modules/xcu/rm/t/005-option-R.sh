# rm -R removes a whole file hierarchy, dot files and empty directories included.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.005
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.005
mkdir -p t/a/b || pc_fail "cannot create tree"
printf x >t/f || pc_fail "cannot create t/f"
printf x >t/a/g || pc_fail "cannot create t/a/g"
printf x >t/.h || pc_fail "cannot create t/.h"
printf x >t/a/b/i || pc_fail "cannot create t/a/b/i"
pc_test
rm -R t >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -R exited with status $rc"
[ ! -e t ] || pc_fail "t still exists"
pc_pass
