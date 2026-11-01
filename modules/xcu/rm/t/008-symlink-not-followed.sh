# rm -r removes a symbolic link in the hierarchy without entering what it points to.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.008
# MUTANT    rm.follow_links
# MUTANT    rm.link_target_removed
# VARIANT   rm.reverse_order
# VARIANT   rm.ref
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.008
mkdir keep d || pc_fail "cannot create directories"
printf x >keep/k || pc_fail "cannot create keep/k"
ln -s ../keep d/l || pc_fail "cannot create symbolic link"
ln -s ../keep/k d/m || pc_fail "cannot create symbolic link to a file"
ln -s keep top || pc_fail "cannot create the operand link"
pc_test
rm -r d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -r exited with status $rc"
[ ! -e d ] || pc_fail "d still exists"
[ -f keep/k ] || pc_fail "the file behind the link was removed"
rm -r top >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm -r on a link operand exited with status $rc"
[ ! -L top ] || pc_fail "the link operand still exists"
[ -f keep/k ] || pc_fail "rm -r followed a link operand"
pc_pass
