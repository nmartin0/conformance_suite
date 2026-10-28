# Without -R, cp treats a source that is a directory as an error: it writes a diagnostic, exits non-zero, and copies nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.004
mkdir sd nd2 || pc_fail "cannot create directories"
printf 'x\n' >sd/f || pc_fail "cannot create sd/f"
pc_test
cp sd nd >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited 0 for a directory source without -R"
[ -s err ] || pc_fail "no diagnostic"
[ ! -e nd ] || pc_fail "a target was created"
cp sd nd2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited 0 for a directory source and a directory target"
[ ! -e nd2/sd ] || pc_fail "the directory was copied into the target"
pc_pass
