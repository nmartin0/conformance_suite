# A directory source without -R is diagnosed and skipped.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.028
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.028
mkdir d tgt || pc_unresolved "cannot create directories"
printf 'x\n' >f || pc_unresolved "cannot create f"
pc_test
cp d f tgt >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited with status 0"
[ -s err ] || pc_fail "no diagnostic written"
[ -e tgt/d ] && pc_fail "tgt/d was created"
[ -f tgt/f ] || pc_fail "tgt/f was not copied"
pc_pass
