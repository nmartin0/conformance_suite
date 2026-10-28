# chgrp sets the group ID of the file to the numeric group operand and leaves the owner alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.001
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.001
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

: >f || pc_fail "cannot create f"
ids f || pc_fail "cannot list f"
u0=$iu
pc_test
chgrp 29998 f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chgrp exited with status $rc"
ids f || pc_fail "cannot list f"
[ "$ig" = 29998 ] || pc_fail "group is $ig, expected 29998"
[ "$iu" = "$u0" ] || pc_fail "owner changed from $u0 to $iu"
pc_pass
