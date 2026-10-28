# chown -R changes the directory operand and everything below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.chown.003
. "$PC_CORE/pc.sh"
pc_begin XCU.chown.003
ids() {
	set -- $(ls -ldn "$1") || return 1
	iu=$3
	ig=$4
}

mkdir t t/sub || pc_fail "cannot create directories"
: >t/f || pc_fail "cannot create t/f"
: >t/sub/g || pc_fail "cannot create t/sub/g"
pc_test
chown -R 29999:29998 t >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "chown -R exited with status $rc"
for n in t t/sub t/f t/sub/g; do
	ids "$n" || pc_fail "cannot list $n"
	[ "$iu" = 29999 ] && [ "$ig" = 29998 ] || pc_fail "$n has owner $iu and group $ig"
done
pc_pass
