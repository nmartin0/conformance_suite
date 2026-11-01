# nl -s sets the characters between number and text; -v sets the first number and -i the step.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.005
# MUTANT    nl.s_ignored
# MUTANT    nl.s_first_only
# MUTANT    nl.v_ignored
# MUTANT    nl.v_off1
# MUTANT    nl.v_zero_default
# MUTANT    nl.i_ignored
# MUTANT    nl.i_off1
# MUTANT    nl.i_first_step
# MUTANT    nl.vi_swapped
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.005
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
printf 'a\nb\nc\n' >in || pc_fail "cannot create input"
pc_test
nl -s '::' in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     1::a${NL}     2::b${NL}     3::c${NL}x" ] || pc_fail "-s '::' is wrong"
nl -s 'xy' in >out 2>/dev/null </dev/null
got=$(numbered xy out; printf x)
[ "$got" = "     1xya${NL}     2xyb${NL}     3xyc${NL}x" ] || pc_fail "-s 'xy' is wrong"
nl -v 10 -i 5 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "    10:a${NL}    15:b${NL}    20:c${NL}x" ] || pc_fail "-v 10 -i 5 is wrong"
nl -v 7 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     7:a${NL}     8:b${NL}     9:c${NL}x" ] || pc_fail "-v 7 alone is wrong"
nl -i 3 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     1:a${NL}     4:b${NL}     7:c${NL}x" ] || pc_fail "-i 3 alone is wrong"
nl -v 0 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     0:a${NL}     1:b${NL}     2:c${NL}x" ] || pc_fail "-v 0 is wrong"
pc_pass
