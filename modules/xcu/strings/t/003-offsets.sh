# strings -t d, o and x precede each string with its offset in that base and a separating blank.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strings.003
. "$PC_CORE/pc.sh"
pc_begin XCU.strings.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\000defgh\000ij\000klmn\nxyz\n' >in || pc_fail "cannot create input"
pc_test
# has FORMAT: the lines of strings -t FORMAT as [offset string]. In the file
# format notation the blank between the two is one or more blanks, a %d
# without a width may be padded with blanks and a %o may have leading zeros,
# so those are removed before the comparison.
has() {
	f=$1
	shift
	strings -a -t "$f" -n 4 in 2>/dev/null </dev/null | while IFS= read -r l; do
		case $f in
		d) l=${l#"${l%%[! 	]*}"} ;;
		o) l=${l#"${l%%[!0]*}"} ;;
		esac
		off=${l%%[ 	]*}
		str=${l#"$off"}
		case $str in
		[' 	']*) str=${str#"${str%%[! 	]*}"}; l="$off $str" ;;
		esac
		printf '[%s]' "$l"
	done
}
got=$(has d)
[ "$got" = "[4 defgh][13 klmn]" ] || pc_fail "-t d did not give '4 defgh' and '13 klmn'"
got=$(has o)
[ "$got" = "[4 defgh][15 klmn]" ] || pc_fail "-t o did not give '4 defgh' and '15 klmn'"
got=$(has x)
[ "$got" = "[4 defgh][d klmn]" ] || pc_fail "-t x did not give '4 defgh' and 'd klmn'"
pc_pass
