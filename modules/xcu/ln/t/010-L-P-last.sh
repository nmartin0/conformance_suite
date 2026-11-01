# When -L and -P are both given, the last one decides.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.010
# MUTANT    ln.L_as_P
# MUTANT    ln.P_as_L
# MUTANT    ln.LP_first_wins
# MUTANT    ln.LP_both_error
# MUTANT    ln.ok_status1
#
# As for ln -P alone, a failing -P link is blamed on the utility only after
# an ordinary setup probe of the same -P link has worked.
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.010
printf 'd\n' >lf10 || pc_fail "cannot create lf10"
ln -s lf10 sl || pc_fail "cannot create symbolic link"
ln -P sl probe >/dev/null 2>&1 </dev/null || pc_unresolved "setup: a hard link to a symbolic link cannot be made here"
pc_test
ln -P -L sl h1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P -L exited with status $rc"
[ ! -h h1 ] || pc_fail "-P -L: h1 is a symbolic link"
[ h1 -ef lf10 ] || pc_fail "-P -L: h1 is not a link to lf10"
ln -L -P sl h2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L -P exited with status $rc"
[ -h h2 ] || pc_fail "-L -P: h2 is not a symbolic link"
ln -L -P -L sl h3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L -P -L exited with status $rc"
[ ! -h h3 ] || pc_fail "-L -P -L: h3 is a symbolic link"
ln -P -L -P sl h4 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -P -L -P exited with status $rc"
[ -h h4 ] || pc_fail "-P -L -P: h4 is not a symbolic link"
pc_pass
