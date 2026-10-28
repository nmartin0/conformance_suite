# nl -d sets the two characters of the section delimiters; a single character leaves the second as a colon.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.008
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.008
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
printf '%s\n' a b '@!@!@!' '@!@!' c >in1 || pc_fail "cannot create input"
printf '%s\n' a b '@:@:@:' '@:@:' c >in2 || pc_fail "cannot create input"
pc_test
nl -d '@!' -s '|' in1 >out 2>/dev/null </dev/null
got=$(numbered '|' out; printf x)
[ "$got" = "     1|a${NL}     2|b${NL}     1|c${NL}x" ] || pc_fail "-d '@!' did not make @!@!@! and @!@! delimiters"
nl -d '@' -s '|' in2 >out 2>/dev/null </dev/null
got=$(numbered '|' out; printf x)
[ "$got" = "     1|a${NL}     2|b${NL}     1|c${NL}x" ] || pc_fail "-d '@' did not make @:@:@: and @:@: delimiters"
pc_pass
