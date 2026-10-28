# nl -l num with -b a numbers only every numth line of a run of blank lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.006
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.006
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
printf 'a\n\n\n\n\nb\n' >in || pc_fail "cannot create input"
pc_test
nl -b a -l 2 -s : in >out 2>/dev/null </dev/null
got=$(numbered : out; printf x)
[ "$got" = "     1:a${NL}     2:${NL}     3:${NL}     4:b${NL}x" ] || pc_fail "-l 2 did not number every second blank line"
pc_pass
