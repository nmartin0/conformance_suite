# realpath -E still fails when a directory in the path does not exist.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.realpath.005
. "$PC_CORE/pc.sh"
pc_begin XCU.realpath.005
root=$(pwd -P) || pc_fail "cannot determine the directory"
mkdir real real/sub || pc_fail "cannot create directories"
: >real/f || pc_fail "cannot create real/f"
ln -s f real/fl || pc_fail "cannot create real/fl"
ln -s real/sub lnk2 || pc_fail "cannot create lnk2"
pc_test
realpath -E real/nodir/new >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "realpath -E exited 0 for a missing directory part"
[ ! -s out ] || pc_fail "output written"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
