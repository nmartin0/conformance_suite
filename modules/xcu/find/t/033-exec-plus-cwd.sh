# find -exec ... {} + runs in the starting directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.033
. "$PC_CORE/pc.sh"
pc_begin XCU.find.033
mkdir -p d/sub || pc_unresolved "cannot create d/sub"
: >d/sub/f || pc_unresolved "cannot create d/sub/f"
here=$(pwd -P) || pc_unresolved "cannot find the working directory"
pc_test
find d -type f -exec sh -c 'pwd -P >cwd.out' sh {} + >out 2>err </dev/null || pc_fail "find: nonzero status: $(cat err)"
[ -f cwd.out ] || pc_fail "the utility did not run in the starting directory (no cwd.out there)"
[ "$(cat cwd.out)" = "$here" ] || pc_fail "the utility ran in '$(cat cwd.out)', expected '$here'"
pc_pass
