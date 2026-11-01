# -n numbers lines (width 5 and a tab by default, or the given width and character), -o indents.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.005
# MUTANT    pr.n_ignored
# MUTANT    pr.n_width4
# MUTANT    pr.n_width6
# MUTANT    pr.n_width_ignored
# MUTANT    pr.n_char_ignored
# MUTANT    pr.n_default_space
# MUTANT    pr.n_start0
# MUTANT    pr.n_stuck
# MUTANT    pr.n_mod10
# MUTANT    pr.o_ignored
# MUTANT    pr.o_plus1
# MUTANT    pr.o_minus1
# MUTANT    pr.o_first_only
# MUTANT    pr.o_tab
# VARIANT   pr.n_zeropad
# VARIANT   pr.n_leftalign
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\n' >f || pc_unresolved "cannot create input"
: >g || pc_unresolved "cannot create input"
_i=1
while [ "$_i" -le 12 ]; do printf 'x%s\n' "$_i" >>g; _i=$((_i+1)); done
TAB=$(printf '\t')
# numline <line> <width> <separator> <number>: the number field of an output
# line must be exactly <width> long, hold the number (padding with blanks or
# zeros, on either side, is not fixed) and be followed by the separator
numline() {
	nl_line=$1
	nl_f=$(printf '%s' "$nl_line" | cut -c1-"$2")
	nl_rest=${nl_line#"$nl_f"}
	[ "${#nl_f}" -eq "$2" ] || return 1
	case $nl_rest in "$3"*) nl_rest=${nl_rest#"$3"};; *) return 1;; esac
	nl_v=$(printf '%s' "$nl_f" | tr -d ' ')
	while :; do case $nl_v in 0?*) nl_v=${nl_v#0};; *) break;; esac; done
	[ "$nl_v" = "$4" ] || return 1
	numline_rest=$nl_rest
}
pc_test
got=$(pr -t -n f 2>/dev/null </dev/null)
numline "$(printf '%s\n' "$got" | sed -n 1p)" 5 "$TAB" 1 && [ "$numline_rest" = a ] || pc_fail "-n did not number the first line with 5 digits and a tab"
numline "$(printf '%s\n' "$got" | sed -n 2p)" 5 "$TAB" 2 && [ "$numline_rest" = b ] || pc_fail "-n did not number the second line with 5 digits and a tab"
got=$(pr -t -n3 f 2>/dev/null </dev/null)
numline "$(printf '%s\n' "$got" | sed -n 1p)" 3 "$TAB" 1 && [ "$numline_rest" = a ] || pc_fail "-n3 did not use a 3-digit number"
numline "$(printf '%s\n' "$got" | sed -n 2p)" 3 "$TAB" 2 && [ "$numline_rest" = b ] || pc_fail "-n3 did not use a 3-digit number on the second line"
got=$(pr -t -n:3 f 2>/dev/null </dev/null)
numline "$(printf '%s\n' "$got" | sed -n 1p)" 3 : 1 && [ "$numline_rest" = a ] || pc_fail "-n:3 did not separate the number with the given character"
numline "$(printf '%s\n' "$got" | sed -n 2p)" 3 : 2 && [ "$numline_rest" = b ] || pc_fail "-n:3 did not separate the second number with the given character"
got=$(pr -t -n g 2>/dev/null </dev/null)
numline "$(printf '%s\n' "$got" | sed -n 12p)" 5 "$TAB" 12 && [ "$numline_rest" = x12 ] || pc_fail "-n did not number the twelfth line 12"
got=$(pr -t -o 4 f 2>/dev/null </dev/null; printf x)
[ "$got" = "    a${NL}    b${NL}x" ] || pc_fail "-o 4 did not precede each line with 4 spaces"
got=$(pr -t -o 7 f 2>/dev/null </dev/null; printf x)
[ "$got" = "       a${NL}       b${NL}x" ] || pc_fail "-o 7 did not precede each line with 7 spaces"
pc_pass
