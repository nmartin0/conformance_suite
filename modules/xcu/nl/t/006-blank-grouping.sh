# nl -l num with -b a numbers only every numth line of a run of blank lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.006
# MUTANT    nl.l_ignored
# MUTANT    nl.l_off1
# MUTANT    nl.l_first
# MUTANT    nl.l_global
# MUTANT    nl.l_text
#
# Runs are never longer than num, so that the choice the text leaves for the
# lines after the numth one in a longer run is not needed. Each output line
# is classified, not compared: a line with the separator is numbered, any
# other is unnumbered (what an unnumbered line looks like is XCU.nl.002's).
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.006
LC_ALL=POSIX
export LC_ALL
NL='
'
shape() {
	sed '/:/!s/.*/-/' "$1"
}
printf 'a\n\nb\nc\n\n\nd\n' >in1 || pc_fail "cannot create input"
printf 'a\n\n\nb\n\n\n\nc\n' >in2 || pc_fail "cannot create input"
pc_test
nl -b a -l 2 -s : in1 >out 2>/dev/null </dev/null
[ -f out ] || pc_fail "no output was captured"
got=$(shape out; printf x)
[ "$got" = "     1:a${NL}-${NL}     2:b${NL}     3:c${NL}-${NL}     4:${NL}     5:d${NL}x" ] || pc_fail "-l 2 did not number exactly the second line of a pair of blank lines"
nl -b a -l 3 -s : in2 >out 2>/dev/null </dev/null
got=$(shape out; printf x)
[ "$got" = "     1:a${NL}-${NL}-${NL}     2:b${NL}-${NL}-${NL}     3:${NL}     4:c${NL}x" ] || pc_fail "-l 3 did not number exactly the third line of a run of three blank lines"
pc_pass
