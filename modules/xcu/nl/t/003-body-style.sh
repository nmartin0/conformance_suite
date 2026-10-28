# nl -b a numbers all lines, -b t only non-empty ones, -b n none, and -b pstring those matching the regular expression.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.003
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.003
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
printf 'a\n\nb\n' >in1 || pc_fail "cannot create input"
printf 'a\nb\nab\nc\n' >in2 || pc_fail "cannot create input"
pc_test
nl -b a in1 >out 2>/dev/null </dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}a${NL}     2${T}${NL}     3${T}b${NL}x" ] || pc_fail "-b a did not number all three lines"
nl -b t in1 >out 2>/dev/null </dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}a${NL}     2${T}b${NL}x" ] || pc_fail "-b t did not number only the non-empty lines"
nl -b n in1 >out 2>/dev/null </dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "x" ] || pc_fail "-b n numbered a line"
nl -b pb in2 >out 2>/dev/null </dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}b${NL}     2${T}ab${NL}x" ] || pc_fail "-b pb did not number exactly the lines containing b"
nl -b 'p^a' in2 >out 2>/dev/null </dev/null
got=$(numbered "$T" out; printf x)
[ "$got" = "     1${T}a${NL}     2${T}ab${NL}x" ] || pc_fail "-b p'^a' did not number exactly the lines starting with a"
pc_pass
