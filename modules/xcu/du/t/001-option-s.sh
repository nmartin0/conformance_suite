# du -s writes exactly one line per operand: a number, a blank and the operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.du.001
. "$PC_CORE/pc.sh"
pc_begin XCU.du.001
mkdir d d/sub || pc_fail "cannot create directories"
printf 'abc\n' >d/a || pc_fail "cannot create d/a"
printf 'def\n' >d/sub/b || pc_fail "cannot create d/sub/b"
pc_test
du -s d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "du -s exited with status $rc"
n=$(sed -n '$=' out)
[ "$n" = 1 ] || pc_fail "du -s wrote $n lines, expected 1"
set -- $(cat out)
[ "$#" -eq 2 ] || pc_fail "line has $# fields"
case $1 in
''|*[!0-9]*) pc_fail "size '$1' is not a number" ;;
esac
[ "$2" = d ] || pc_fail "name is '$2', expected d"
pc_pass
