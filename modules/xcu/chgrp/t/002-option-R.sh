# chgrp -R changes the directory operand and everything below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chgrp.002
. "$PC_CORE/pc.sh"
pc_begin XCU.chgrp.002
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

mkdir t t/sub || pc_fail "cannot create directories"
: >t/f || pc_fail "cannot create t/f"
: >t/sub/g || pc_fail "cannot create t/sub/g"
pc_test
chgrp -R 29998 t >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chgrp -R exited with status $rc"
for n in t t/sub t/f t/sub/g; do
	ids "$n" || pc_fail "cannot list $n"
	[ "$ig" = 29998 ] || pc_fail "$n has group $ig"
done
pc_pass
