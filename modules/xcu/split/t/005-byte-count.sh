# split -b n writes pieces of n bytes, and -b nk pieces of n times 1024 bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.005
. "$PC_CORE/pc.sh"
pc_begin XCU.split.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij' >in1 || pc_fail "cannot create input"
eight=abcdefgh
k=
i=0
while [ $i -lt 128 ]; do
	k=$k$eight
	i=$((i + 1))
done
printf '%sZ' "$k" >in2 || pc_fail "cannot create input"
pc_test
split -b 4 in1 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -b 4 gave status $rc"
[ "$(cat xaa)" = abcd ] || pc_fail "xaa is not abcd"
[ "$(cat xab)" = efgh ] || pc_fail "xab is not efgh"
[ "$(cat xac)" = ij ] || pc_fail "xac is not ij"
[ -e xad ] && pc_fail "a fourth file was created"
rm -f xaa xab xac
split -b 1k in2 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -b 1k gave status $rc"
[ "$(cat xaa)" = "$k" ] || pc_fail "xaa is not the first 1024 bytes"
[ "$(cat xab)" = Z ] || pc_fail "xab is not the last byte"
[ -e xac ] && pc_fail "a third file was created for -b 1k"
pc_pass
