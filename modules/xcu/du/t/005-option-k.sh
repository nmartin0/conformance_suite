# du -k reports 1024-byte units, so its number is the 512-byte number divided by two, rounded up.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.005
. "$PC_CORE/pc.sh"
pc_begin XCU.du.005
mkdir d || pc_fail "cannot create d"
i=0
while [ "$i" -lt 1600 ]; do
	printf '%063d\n' "$i"
	i=$((i + 1))
done >d/f || pc_fail "cannot create d/f"
pc_test
du -s d >out 2>err </dev/null
set -- $(cat out)
n512=$1
du -k -s d >out 2>err </dev/null
set -- $(cat out)
n1024=$1
case $n512$n1024 in
''|*[!0-9]*) pc_fail "sizes '$n512' and '$n1024' are not numbers" ;;
esac
[ "$n1024" -eq $(((n512 + 1) / 2)) ] || pc_fail "512-byte units: $n512, 1024-byte units: $n1024"
[ "$n1024" -ge 1 ] || pc_fail "102400 bytes of data are reported as $n1024 KiB"
pc_pass
