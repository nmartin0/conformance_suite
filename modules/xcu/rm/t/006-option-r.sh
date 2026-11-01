# rm -r is equivalent to rm -R.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.006
# MUTANT    rm.r_ignored
# MUTANT    rm.R_no_dotfiles
# MUTANT    rm.R_no_empty_dirs
# MUTANT    rm.R_shallow
# MUTANT    rm.R_status1
# MUTANT    rm.R_leaves_top
# VARIANT   rm.reverse_order
# VARIANT   rm.ref
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.006
mkdir -p t/a/b t/e t/a/e2 || pc_fail "cannot create tree"
printf x >t/f || pc_fail "cannot create t/f"
printf x >t/a/g || pc_fail "cannot create t/a/g"
printf x >t/.h || pc_fail "cannot create t/.h"
printf x >t/a/b/i || pc_fail "cannot create t/a/b/i"
pc_test
rm -r t >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -r exited with status $rc"
[ ! -e t ] || pc_fail "t still exists"
pc_pass
