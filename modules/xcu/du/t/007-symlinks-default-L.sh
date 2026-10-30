# By default du counts a symbolic link itself and does not follow it; -L follows links in the hierarchy.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.007
. "$PC_CORE/pc.sh"
pc_begin XCU.du.007
mkdir bigdir d || pc_fail "cannot create directories"
pcl=000000000000000000000000000000000000000000000000000000000000000
i=0
while [ "$i" -lt 9375 ]; do
	echo "$pcl"
	i=$((i + 1))
done >bigdir/big || pc_fail "cannot create bigdir/big"
ln -s ../bigdir d/lnk || pc_fail "cannot create symbolic link"
pc_test
du -k -s d >out 2>err </dev/null
set -- $(cat out)
case $1 in
''|*[!0-9]*) pc_fail "size '$1' is not a number" ;;
esac
ndef=$1
du -k -L -s d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -L exited with status $rc"
set -- $(cat out)
case $1 in
''|*[!0-9]*) pc_fail "size '$1' is not a number" ;;
esac
[ "$1" -gt "$ndef" ] || pc_fail "du -L ($1) did not count more than the default ($ndef): the link was followed by default, or -L did not follow it"
pc_pass
