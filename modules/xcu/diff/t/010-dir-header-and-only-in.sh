# diff in directory mode: header line and "Only in" with a relative directory pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.010
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.010
LC_ALL=POSIX
export LC_ALL
mkdir d1 d2 d1/sub d2/sub || pc_unresolved "cannot create directories"
printf 'one\n' >d1/dif && printf 'two\n' >d2/dif || pc_unresolved "cannot create input"
printf 'p\n' >d1/sub/f && printf 'p\n' >d2/sub/f && printf 'z\n' >d2/sub/z || pc_unresolved "cannot create input"
pc_test
diff -b d1 d2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "diff -b d1 d2: exit status $rc, expected 1"
grep '^diff -b ' out >/dev/null || pc_fail "no header line beginning 'diff -b ': $(cat out)"
l=$(grep '^diff -b ' out | head -1)
case $l in
*dif*dif*) ;;
*) pc_fail "the header line does not name the two files: '$l'" ;;
esac
diff -r d1 d2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "diff -r d1 d2: exit status $rc, expected 1"
grep -x 'Only in d2/sub: z' out >/dev/null || pc_fail "no 'Only in d2/sub: z' line: $(cat out)"
pc_pass
