# With -k, the block size for -s and for the total line of the long format is 1024 bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.018
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.018
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
i=0
while [ "$i" -lt 4096 ]; do
	printf '%063d\n' "$i"
	i=$((i + 1))
done >d/big || pc_fail "cannot create d/big"
pc_test
ls -ks d/big >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -ks exited with status $rc"
set -- $(cat out)
case $1 in
''|*[!0-9]*) pc_fail "block count '$1' is not a number" ;;
esac
[ "$1" -ge 1 ] || pc_fail "the block count of a non-empty file is $1"
t512=$(ls -l d | sed -n 1p)
t1024=$(ls -lk d | sed -n 1p)
t512=${t512#total }
t1024=${t1024#total }
case $t512$t1024 in
''|*[!0-9]*) pc_fail "totals '$t512' and '$t1024' are not numbers" ;;
esac
[ "$t1024" -eq $(((t512 + 1) / 2)) ] || pc_fail "total is $t512 in 512-byte units but $t1024 with -k"
pc_pass
