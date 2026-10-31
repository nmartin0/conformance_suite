# tr understands the character classes: case conversion with lower and upper, and any class when deleting or squeezing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.007
# MUTANT    tr.alpha_nolower
# MUTANT    tr.digit_hex
# MUTANT    tr.alnum_nodigit
# MUTANT    tr.xdigit_nolower
# MUTANT    tr.space_no_blank
# MUTANT    tr.space_no_tab
# MUTANT    tr.space_no_nl
# MUTANT    tr.space_no_vt
# MUTANT    tr.space_no_ff
# MUTANT    tr.space_no_cr
# MUTANT    tr.blank_nl
# MUTANT    tr.punct_nounder
# MUTANT    tr.cntrl_nodel
# MUTANT    tr.graph_space
# MUTANT    tr.print_nospace
# MUTANT    tr.upper_nz
# MUTANT    tr.lower_nz
# MUTANT    tr.case_partial
# MUTANT    tr.l2u_ignored
# MUTANT    tr.u2l_ignored
# MUTANT    tr.sq_mixed
# MUTANT    tr.sq_s1
# MUTANT    tr.no_final_newline
# VARIANT   tr.class_reverse
# VARIANT   tr.pad_last
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.007
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'Hello World 1\n' >in1 || pc_fail "cannot create input"
printf 'a1b22c\n' >in2 || pc_fail "cannot create input"
printf 'a   b\t\tc\n\n\nd \te\n' >in3 || pc_fail "cannot create input"
printf 'AAbbCc\n' >in4 || pc_fail "cannot create input"
pc_test
got=$(tr '[:lower:]' '[:upper:]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "HELLO WORLD 1${NL}rc=0" ] || pc_fail "[:lower:] to [:upper:] gave the wrong result"
got=$(tr '[:upper:]' '[:lower:]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello world 1${NL}rc=0" ] || pc_fail "[:upper:] to [:lower:] gave the wrong result"
got=$(tr -d '[:digit:]' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "tr -d '[:digit:]' gave the wrong result"
got=$(tr -s '[:space:]' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${T}c${NL}d ${T}e${NL}rc=0" ] || pc_fail "tr -s '[:space:]' gave the wrong result"
got=$(tr -s '[:upper:]' '[:lower:]' <in4 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "tr -s '[:upper:]' '[:lower:]' gave the wrong result"
# each class holds the characters of the POSIX locale and no others
inclass() {
	case $1 in
	upper) [ "$2" -ge 65 ] && [ "$2" -le 90 ] ;;
	lower) [ "$2" -ge 97 ] && [ "$2" -le 122 ] ;;
	alpha) inclass upper "$2" || inclass lower "$2" ;;
	digit) [ "$2" -ge 48 ] && [ "$2" -le 57 ] ;;
	alnum) inclass alpha "$2" || inclass digit "$2" ;;
	xdigit) inclass digit "$2" || { [ "$2" -ge 65 ] && [ "$2" -le 70 ]; } || { [ "$2" -ge 97 ] && [ "$2" -le 102 ]; } ;;
	space) [ "$2" -eq 32 ] || { [ "$2" -ge 9 ] && [ "$2" -le 13 ]; } ;;
	blank) [ "$2" -eq 32 ] || [ "$2" -eq 9 ] ;;
	punct) [ "$2" -ge 33 ] && [ "$2" -le 126 ] && ! inclass alnum "$2" ;;
	cntrl) [ "$2" -lt 32 ] || [ "$2" -eq 127 ] ;;
	graph) [ "$2" -ge 33 ] && [ "$2" -le 126 ] ;;
	print) [ "$2" -ge 32 ] && [ "$2" -le 126 ] ;;
	esac
}
i=1
: >allascii
while [ "$i" -le 127 ]; do
	printf "\\$(printf %03o "$i")" >>allascii
	i=$((i + 1))
done
for c in upper lower alpha digit alnum xdigit space blank punct cntrl graph print; do
	: >"exp.$c"
	i=1
	while [ "$i" -le 127 ]; do
		inclass "$c" "$i" || printf "\\$(printf %03o "$i")" >>"exp.$c"
		i=$((i + 1))
	done
	tr -d "[:$c:]" <allascii >"out.$c" 2>/dev/null
	rc=$?
	[ -f "out.$c" ] || pc_fail "tr -d '[:$c:]' left no output"
	[ "$rc" -eq 0 ] || pc_fail "tr -d '[:$c:]' exited $rc"
	cmp -s "out.$c" "exp.$c" || pc_fail "tr -d '[:$c:]' deleted the wrong characters"
done
pc_pass
