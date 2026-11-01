# With -l, differing lengths give the EOF diagnostic after any differences have been listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.006
# MUTANT    cmp.l_eof_nodiag
# MUTANT    cmp.l_eof_exit0
# MUTANT    cmp.l_eof_exit2
# MUTANT    cmp.l_eof_nodiffs
# MUTANT    cmp.l_eof_swap_name
# MUTANT    cmp.l_eof_stdout
# MUTANT    cmp.l_eof_only_first
# MUTANT    cmp.eof_info_nl
# VARIANT   cmp.eof_noinfo
# VARIANT   cmp.eof_info_words
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.006
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

printf 'abc' >f1 || pc_fail "cannot create f1"
printf 'abcd' >f2 || pc_fail "cannot create f2"
printf 'aXcY' >g1 || pc_fail "cannot create g1"
printf 'abcde' >g2 || pc_fail "cannot create g2"
pc_test
# chk FILE NAME: FILE holds one newline-terminated line, "cmp: EOF on NAME"
# alone or followed by a blank and more.
chk() {
	chk_e=
	chk_x=
	{
		IFS= read -r chk_e || pc_fail "the diagnostic is empty or lacks its newline"
		IFS= read -r chk_x
		[ -z "$chk_x" ] || pc_fail "the diagnostic has more than one line"
	} <"$1"
	case $chk_e in
	"cmp: EOF on $2" | "cmp: EOF on $2 "*) ;;
	*) pc_fail "diagnostic is: $chk_e" ;;
	esac
}
cmp -l f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc, expected 1"
chk err f1
cmp -l g1 g2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l (second case) exited with status $rc, expected 1"
lfields out
[ "$data" = "2 130 142${nl}4 131 144${nl}" ] || pc_fail "output is: $data"
chk err g1
# the shorter file second: it is the one named
cmp -l g2 g1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l (reversed) exited with status $rc, expected 1"
lfields out
[ "$data" = "2 142 130${nl}4 144 131${nl}" ] || pc_fail "reversed output is: $data"
chk err g1
pc_pass
