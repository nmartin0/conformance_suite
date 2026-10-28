# od -t o1, x1, u1 and d1 write each byte as an octal, hexadecimal, unsigned or signed decimal number; sizes 2, 4 and 8 group bytes, with the byte order left to the implementation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.002
. "$PC_CORE/pc.sh"
pc_begin XCU.od.002
LC_ALL=POSIX
export LC_ALL
NL='
'
norm() {
	normout=
	for t in "$@"; do
		z=${t%%[!0]*}
		t=${t#"$z"}
		[ -z "$t" ] && t=0
		normout="$normout $t"
	done
	normout=${normout# }
}
printf 'abcdef' >f1 || pc_fail "cannot create input"
printf 'aabbcc' >f2 || pc_fail "cannot create input"
printf 'aaaabbbb' >f4 || pc_fail "cannot create input"
printf 'aaaaaaaa' >f8 || pc_fail "cannot create input"
pc_test
got=$(od -A n -v -t o1 f1 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "141 142 143 144 145 146" ] || pc_fail "-t o1 is wrong"
got=$(od -A n -v -t x1 f1 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "61 62 63 64 65 66" ] || pc_fail "-t x1 is wrong"
got=$(od -A n -v -t u1 f1 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "97 98 99 100 101 102" ] || pc_fail "-t u1 is wrong"
got=$(od -A n -v -t d1 f1 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "97 98 99 100 101 102" ] || pc_fail "-t d1 is wrong"
got=$(od -A n -v -t x2 f2 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "6161 6262 6363" ] || pc_fail "-t x2 is wrong"
got=$(od -A n -v -t u2 f2 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "24929 25186 25443" ] || pc_fail "-t u2 is wrong"
got=$(od -A n -v -t o2 f2 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "60541 61142 61543" ] || pc_fail "-t o2 is wrong"
got=$(od -A n -v -t x4 f4 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "61616161 62626262" ] || pc_fail "-t x4 is wrong"
got=$(od -A n -v -t x8 f8 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "6161616161616161" ] || pc_fail "-t x8 is wrong"
pc_pass
