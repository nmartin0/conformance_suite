# od -t o1, x1, u1 and d1 write each byte as an octal, hexadecimal, unsigned or signed decimal number; sizes 2, 4 and 8 group bytes, with the byte order left to the implementation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.002
# MUTANT    od.sz8_to_4
# MUTANT    od.sz4_to_2
# MUTANT    od.sz2_to_1
# MUTANT    od.o_as_x
# MUTANT    od.x_as_o
# MUTANT    od.x_as_u
# MUTANT    od.o_as_u
# MUTANT    od.u_as_d
# MUTANT    od.d_as_u
# MUTANT    od.partial_dropped
# MUTANT    od.partial_ff
# VARIANT   od.endian_swap
# VARIANT   od.gap3
# VARIANT   od.hex_upper
# VARIANT   od.pad_none
# VARIANT   od.an_empty
# VARIANT   od.trail_blank
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
# bytes with the high bit set tell signed from unsigned numbers
printf '\377\377\377\377\377\377\377\377' >ff || pc_fail "cannot create input"
printf '\200\200' >hi2 || pc_fail "cannot create input"
# a length that is not a multiple of the item size
printf 'aaa' >f3 || pc_fail "cannot create input"
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
got=$(od -A n -v -t u1 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "255 255 255 255 255 255 255 255" ] || pc_fail "-t u1 of 0xff bytes is not 255"
got=$(od -A n -v -t d1 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "-1 -1 -1 -1 -1 -1 -1 -1" ] || pc_fail "-t d1 of 0xff bytes is not -1"
got=$(od -A n -v -t d2 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "-1 -1 -1 -1" ] || pc_fail "-t d2 of 0xffff is not -1"
got=$(od -A n -v -t u2 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "65535 65535 65535 65535" ] || pc_fail "-t u2 of 0xffff is not 65535"
got=$(od -A n -v -t u2 hi2 2>/dev/null </dev/null)
set -- $got
[ "$*" = "32896" ] || pc_fail "-t u2 of 0x8080 is not 32896"
got=$(od -A n -v -t d2 hi2 2>/dev/null </dev/null)
set -- $got
[ "$*" = "-32640" ] || pc_fail "-t d2 of 0x8080 is not -32640"
got=$(od -A n -v -t d4 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "-1 -1" ] || pc_fail "-t d4 of 0xffffffff is not -1"
got=$(od -A n -v -t u4 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "4294967295 4294967295" ] || pc_fail "-t u4 of 0xffffffff is not 4294967295"
got=$(od -A n -v -t d8 ff 2>/dev/null </dev/null)
set -- $got
[ "$*" = "-1" ] || pc_fail "-t d8 of eight 0xff bytes is not -1"
got=$(od -A n -v -t o8 f8 2>/dev/null </dev/null)
set -- $got
norm "$@"
[ "$normout" = "605413026054130260541" ] || pc_fail "-t o8 is wrong"
# input that stops inside an item is extended with null bytes: three bytes
# are two two-byte items, the second being 0x61 and a null in either order
got=$(od -A n -v -t x2 f3 2>/dev/null </dev/null)
set -- $got
norm "$@"
case $normout in
"6161 61" | "6161 6100") ;;
*) pc_fail "a final partial item is not the remaining byte extended with null bytes" ;;
esac
pc_pass
