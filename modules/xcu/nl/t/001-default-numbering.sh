# By default nl numbers the non-empty lines, right-justified in six columns, followed by a tab.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.001
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
numbered() {
	while IFS= read -r l; do
		case $l in
		*"$1"*) printf '%s\n' "$l" ;;
		esac
	done <"$2"
}
printf 'a\nb\nc\n' >in || pc_fail "cannot create input"
pc_test
nl in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nl gave status $rc"
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}a${NL}     2${T}b${NL}     3${T}c${NL}x" ] || pc_fail "the numbered lines are not '%6d', tab, text"
nl <in >out 2>/dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}a${NL}     2${T}b${NL}     3${T}c${NL}x" ] || pc_fail "nl did not read standard input"
pc_pass
