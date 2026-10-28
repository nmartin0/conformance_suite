# If one operand cannot be changed, chgrp diagnoses it, handles the others, and exits non-zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.006
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.006
: >f || pc_fail "cannot create f"
gid=$(id -g) || pc_fail "cannot determine the group ID"
pc_test
chgrp "$gid" nonexistent f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chgrp exited 0 although a file does not exist"
[ -s err ] || pc_fail "no diagnostic"
pc_pass
