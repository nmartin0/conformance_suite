# cmp -l writes, for each differing byte, its number in decimal and the two bytes in octal, and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.003
# MUTANT    cmp.l_decimal
# MUTANT    cmp.l_hex
# MUTANT    cmp.l_swapped
# MUTANT    cmp.l_byte0
# MUTANT    cmp.l_octal_byte
# MUTANT    cmp.l_only_first
# MUTANT    cmp.l_skip_last
# MUTANT    cmp.l_cap100
# MUTANT    cmp.l_exit0
# MUTANT    cmp.l_stderr
# MUTANT    cmp.l_no_newline
# MUTANT    cmp.l_extra_default
# MUTANT    cmp.l_ident_exit1
# MUTANT    cmp.ident_out
# MUTANT    cmp.l_high_masked
# MUTANT    cmp.l_block_edge
# MUTANT    cmp.l_mod64k
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.003
nl='
'
# lfields FILE: data = the lines of FILE as "N O1 O2" with single spaces. The
# file format notation lets a blank in the format stand for one or more
# blanks, a %d without a width be padded with blanks and a %o be preceded by
# zeros, so the fields are split on blanks and leading zeros of the octal
# fields are dropped. A line without three fields, or a missing final
# newline, is kept visibly in data.
lfields() {
	data=
	lf_n=
	while read -r lf_n lf_a lf_b lf_x; do
		if [ -z "$lf_b" ] || [ -n "$lf_x" ]; then
			data="$data<bad:$lf_n $lf_a $lf_b $lf_x>$nl"
		else
			lf_a=${lf_a#"${lf_a%%[!0]*}"}
			lf_b=${lf_b#"${lf_b%%[!0]*}"}
			data="$data$lf_n ${lf_a:-0} ${lf_b:-0}$nl"
		fi
		lf_n=
	done <"$1"
	[ -z "$lf_n" ] || data="$data$lf_n<noeol>"
	return 0
}

# rep N C: N copies of the character C (N may be 0).
rep() {
	rep_blk=$2$2$2$2$2$2$2$2$2$2
	rep_blk=$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk$rep_blk
	rep_q=$(($1 / 100))
	rep_r=$(($1 % 100))
	rep_i=0
	while [ "$rep_i" -lt "$rep_q" ]; do
		printf '%s' "$rep_blk"
		rep_i=$((rep_i + 1))
	done
	printf "%.${rep_r}s" "$rep_blk"
}

printf 'abcdef' >f1 || pc_fail "cannot create f1"
printf 'aXcYef' >f2 || pc_fail "cannot create f2"
# many differences: the list is not cut short
rep 300 a >m1 || pc_fail "cannot create m1"
rep 300 b >m2 || pc_fail "cannot create m2"
# a large pair that differs in single bytes at, and next to, places where a
# buffer or a 16-bit count would end (4096, 8192, 65536) and at the last byte
rep 70700 a >p1 || pc_fail "cannot create p1"
{
	rep 4095 a; printf b; printf b; rep 4094 a; printf b; rep 57343 a; printf b; printf b
	rep 5162 a; printf b
} >p2 || pc_fail "cannot create p2"
printf '\377a\200' >q1 || pc_fail "cannot create q1"
printf '\000a\001' >q2 || pc_fail "cannot create q2"
pc_test
cmp -l f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc, expected 1"
lfields out
[ "$data" = "2 142 130${nl}4 144 131${nl}" ] || pc_fail "output is: $data"
# identical files: nothing is listed and the status is 0
cmp -l f1 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp -l exited with status $rc for identical files"
[ ! -s out ] || pc_fail "cmp -l wrote output for identical files"
cmp -l m1 m2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc for 300 differences, expected 1"
lfields out
want=
n=1
while [ "$n" -le 300 ]; do
	want="$want$n 141 142$nl"
	n=$((n + 1))
done
[ "$data" = "$want" ] || pc_fail "output for 300 differences is wrong"
cmp -l p1 p2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc for large files, expected 1"
lfields out
[ "$data" = "4096 141 142${nl}4097 141 142${nl}8192 141 142${nl}65536 141 142${nl}65537 141 142${nl}70700 141 142${nl}" ] || pc_fail "output for large files is: $data"
# bytes of every value: 255 and 128 against 0 and 1
cmp -l q1 q2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc for high bytes, expected 1"
lfields out
[ "$data" = "1 377 0${nl}3 200 1${nl}" ] || pc_fail "output for high bytes is: $data"
pc_pass
