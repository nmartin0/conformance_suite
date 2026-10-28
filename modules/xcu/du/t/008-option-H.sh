# du -H follows a symbolic link given as operand but not one inside the hierarchy.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.008
. "$PC_CORE/pc.sh"
pc_begin XCU.du.008
mkdir bigdir d || pc_fail "cannot create directories"
i=0
while [ "$i" -lt 9375 ]; do
	printf '%063d\n' "$i"
	i=$((i + 1))
done >bigdir/big || pc_fail "cannot create bigdir/big"
ln -s bigdir lbig || pc_fail "cannot create operand link"
ln -s ../bigdir d/lnk || pc_fail "cannot create symbolic link"
pc_test
du -k -s lbig >out 2>err </dev/null
set -- $(cat out)
case $1 in
''|*[!0-9]*) pc_fail "size '$1' is not a number" ;;
esac
nlink=$1
du -k -s d >out 2>err </dev/null
set -- $(cat out)
ndir=$1
du -k -H -s lbig >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -H exited with status $rc"
set -- $(cat out)
case $1 in
''|*[!0-9]*) pc_fail "size '$1' is not a number" ;;
esac
[ "$1" -gt "$nlink" ] || pc_fail "du -H ($1) did not count more than the default ($nlink): the operand link was followed by default, or -H did not follow it"
du -k -H -s d >out 2>err </dev/null
set -- $(cat out)
[ "$1" = "$ndir" ] || pc_fail "du -H changed the size of a hierarchy without operand links from $ndir to $1: it followed a link inside the hierarchy"
pc_pass
