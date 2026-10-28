# With -R, -H follows a symbolic link given as operand to a directory and changes the files below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.005
#
# What -H does to a link met inside the hierarchy is not stated, so only
# the operand link is checked.
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.005
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

mkdir real || pc_fail "cannot create real"
: >real/f || pc_fail "cannot create real/f"
ln -s real top || pc_fail "cannot create top link"
pc_test
chgrp -R -H 29999 top >out2 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chgrp -R -H exited with status $rc"
ids real/f || pc_fail "cannot list real/f"
[ "$ig" = 29999 ] || pc_fail "real/f has group $ig, the operand link was not followed"
ids real || pc_fail "cannot list real"
[ "$ig" = 29999 ] || pc_fail "real has group $ig, the operand link was not followed"
pc_pass
