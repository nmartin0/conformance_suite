# A group operand that is no group name and not numeric is an error and changes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.007
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.007
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
ids f || pc_fail "cannot list f"
g0=$ig
pc_test
chgrp pc_no_such_group_9z8y7 f >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "chgrp exited 0 for an unknown group name"
[ -s err ] || pc_fail "no diagnostic"
ids f || pc_fail "cannot list f"
[ "$ig" = "$g0" ] || pc_fail "the group was changed"
pc_pass
