# cp -R -P -L: the last of -H/-L/-P wins; the link is followed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.025
# MUTANT    cp.R_status1
# MUTANT    cp.sym_first_wins
# MUTANT    cp.sym_P_wins
# MUTANT    cp.sym_multi_error
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.025
mkdir src || pc_unresolved "cannot create src"
printf 'x\n' >src/f || pc_unresolved "cannot create src/f"
ln -s f src/l || pc_unresolved "cannot create symbolic link"
pc_test
cp -R -P -L src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
[ -f dst/l ] && [ ! -h dst/l ] || pc_fail "dst/l is not a regular file"
[ "$(cat dst/l)" = x ] || pc_fail "dst/l has wrong contents"
pc_pass
