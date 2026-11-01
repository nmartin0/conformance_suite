# grep selects lines that contain a match of a basic regular expression anywhere, and a null pattern matches every line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.001
# MUTANT    grep.bre_literal
# MUTANT    grep.bre_as_ere
# MUTANT    grep.anchored_start
# MUTANT    grep.anchored_end
# MUTANT    grep.B_null_dropped
# MUTANT    grep.null_error
# MUTANT    grep.stdin_ignored
# MUTANT    grep.no_final_newline
# MUTANT    grep.trailing_space
# MUTANT    grep.blank_after
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'apple\nbanana\ncherry\n' >in || pc_fail "cannot create input"
pc_test
got=$(grep an in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "banana${NL}rc=0" ] || pc_fail "'an' did not select exactly banana"
got=$(grep '^c.*y$' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "cherry${NL}rc=0" ] || pc_fail "anchored pattern did not select exactly cherry"
got=$(grep 'p\{2\}' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "apple${NL}rc=0" ] || pc_fail "interval expression in a BRE did not select exactly apple"
got=$(grep '' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "apple${NL}banana${NL}cherry${NL}rc=0" ] || pc_fail "a null pattern did not select every line"
got=$(grep an <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "banana${NL}rc=0" ] || pc_fail "with no file operand the standard input was not searched"
pc_pass
