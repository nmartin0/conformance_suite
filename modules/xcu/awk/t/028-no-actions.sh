# a program without patterns or actions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.028
# A valid program that has no pattern and no action does not read its file
# operands and exits with status 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.028
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
# awk must be present and working: a program with an action prints
g=$(awk 'BEGIN { print "pcok" }' 2>/dev/null </dev/null)
[ "$g" = pcok ] || pc_fail "awk did not run a minimal program (got '$g')"
for prog in '' '# only a comment' 'function f(x) { return x }'; do
	awk "$prog" pc-no-such-file >o 2>/dev/null </dev/null; st=$?
	[ "$st" -eq 0 ] || pc_fail "program [$prog]: exit status $st although there is nothing to run"
	[ -s o ] && pc_fail "program [$prog]: it wrote output"
done
pc_pass
