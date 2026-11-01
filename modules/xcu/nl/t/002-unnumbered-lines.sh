# A line that is not numbered is written as the input line alone, with no number and no separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.002
# MUTANT    nl.unnum_padded
# MUTANT    nl.unnum_sep
# MUTANT    nl.unnum_spaces
# MUTANT    nl.unnum_dropped
# MUTANT    nl.default_a
# VARIANT   nl.delim_dropped
# VARIANT   nl.delim_echo
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
only() {
	while IFS= read -r l; do
		case $l in
		*"$1"*) printf '%s\n' "$l" ;;
		esac
	done <"$2"
}
printf 'a\n\nb\n' >in || pc_fail "cannot create input"
printf 'a\nb\nab\n' >in2 || pc_fail "cannot create input"
printf '%s\n' '\:\:\:' HDR '\:\:' BODY '\:' FTR >in3 || pc_fail "cannot create input"
pc_test
got=$(nl in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "     1${T}a${NL}${NL}     2${T}b${NL}rc=0" ] || pc_fail "the empty line was not written as an empty line"
got=$(nl -b n in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}${NL}b${NL}rc=0" ] || pc_fail "-b n did not leave the lines unchanged"
got=$(nl -b pa -s : in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "     1:a${NL}b${NL}     2:ab${NL}rc=0" ] || pc_fail "a line the pattern does not select was not written alone"
nl -s : in3 >out 2>/dev/null </dev/null
[ -f out ] || pc_fail "no output was captured"
got=$(only HDR out; printf x)
[ "$got" = "HDR${NL}x" ] || pc_fail "an unnumbered header line was not written alone"
got=$(only FTR out; printf x)
[ "$got" = "FTR${NL}x" ] || pc_fail "an unnumbered footer line was not written alone"
pc_pass
