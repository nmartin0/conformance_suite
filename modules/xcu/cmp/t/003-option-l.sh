# cmp -l writes, for each differing byte, its number in decimal and the two bytes in octal, and exits 1.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.003
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

printf 'abcdef' >f1 || pc_fail "cannot create f1"
printf 'aXcYef' >f2 || pc_fail "cannot create f2"
pc_test
cmp -l f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc, expected 1"
lfields out
[ "$data" = "2 142 130${nl}4 144 131${nl}" ] || pc_fail "output is: $data"
pc_pass
