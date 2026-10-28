# nl -s sets the characters between number and text; -v sets the first number and -i the step.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.005
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
nl -v 10 -i 5 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "    10:a${NL}    15:b${NL}    20:c${NL}x" ] || pc_fail "-v 10 -i 5 is wrong"
pc_pass
