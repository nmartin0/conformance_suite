# nl -d sets the two characters of the section delimiters; a single character leaves the second as a colon.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.008
# MUTANT    nl.d_ignored
# MUTANT    nl.d_swapped
# MUTANT    nl.d_single_dup
# MUTANT    nl.d_single_bs
# MUTANT    nl.d_single_ignored
# MUTANT    nl.d_adds_default
# MUTANT    nl.hdr_delim_as_body
# MUTANT    nl.ftr_delim_ignored
# MUTANT    nl.body_delim_ignored
# VARIANT   nl.delim_dropped
# VARIANT   nl.delim_echo
#
# Header and footer lines are unnumbered by default, so a header or footer
# delimiter that is not recognized shows as a numbered line.
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
printf '%s\n' a b '@!@!@!' h1 '@!@!' c '\:\:' '@!' f1 >in1 || pc_fail "cannot create input"
printf '%s\n' a b '@:@:@:' h1 '@:@:' c '\:\:' '@:' f1 >in2 || pc_fail "cannot create input"
pc_test
nl -d '@!' -s '|' in1 >out 2>/dev/null </dev/null
[ -f out ] || pc_fail "no output was captured"
got=$(numbered '|' out; printf x)
[ "$got" = '     1|a
     2|b
     1|c
     2|\:\:
x' ] || pc_fail "-d '@!' did not make @!@!@!, @!@! and @! the delimiters (and only those)"
nl -d '@' -s '|' in2 >out 2>/dev/null </dev/null
got=$(numbered '|' out; printf x)
[ "$got" = '     1|a
     2|b
     1|c
     2|\:\:
x' ] || pc_fail "-d '@' did not make @:@:@:, @:@: and @: the delimiters (and only those)"
pc_pass
