# nl -n ln, rn and rz left-justify, right-justify or zero-fill the number in the -w width (six by default).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.004
# MUTANT    nl.n_ln_right
# MUTANT    nl.n_rn_zero
# MUTANT    nl.n_rz_space
# MUTANT    nl.n_ignored
# MUTANT    nl.w_ignored
# MUTANT    nl.w_plus1
# MUTANT    nl.w_minus1
# MUTANT    nl.w_ln_ignored
# MUTANT    nl.w_rz_ignored
# MUTANT    nl.ln_fixed_pad
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.004
LC_ALL=POSIX
export LC_ALL
NL='
'
numbered() {
	while IFS= read -r l; do
		case $l in
		*"$1"*) printf '%s\n' "$l" ;;
		esac
	done <"$2"
}
printf 'a\nb\n' >in || pc_fail "cannot create input"
pc_test
nl -n ln -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "1     :a${NL}2     :b${NL}x" ] || pc_fail "-n ln is wrong"
nl -n rn -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     1:a${NL}     2:b${NL}x" ] || pc_fail "-n rn is wrong"
nl -n rz -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "000001:a${NL}000002:b${NL}x" ] || pc_fail "-n rz is wrong"
nl -n rz -w 3 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "001:a${NL}002:b${NL}x" ] || pc_fail "-n rz -w 3 is wrong"
nl -n rn -w 4 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "   1:a${NL}   2:b${NL}x" ] || pc_fail "-n rn -w 4 is wrong"
nl -n ln -w 2 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "1 :a${NL}2 :b${NL}x" ] || pc_fail "-n ln -w 2 is wrong"
nl -n ln -v 9 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "9     :a${NL}10    :b${NL}x" ] || pc_fail "-n ln with a one-digit then a two-digit number is wrong"
nl -n rn -v 9 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     9:a${NL}    10:b${NL}x" ] || pc_fail "-n rn with a one-digit then a two-digit number is wrong"
nl -n rz -v 9 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "000009:a${NL}000010:b${NL}x" ] || pc_fail "-n rz with a one-digit then a two-digit number is wrong"
pc_pass
