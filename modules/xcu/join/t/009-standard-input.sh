# A - operand stands for standard input in place of file1 or file2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.009
# MUTANT    join.dash_f1_literal
# MUTANT    join.dash_f2_literal
# MUTANT    join.dash_f2_ignored
# MUTANT    join.stdin_swapped
# MUTANT    join.stdin_always_read
#
# Standard input is a pipe in one check and a file in the others; when no
# operand is - it must be left unread.
. "$PC_CORE/pc.sh"
pc_begin XCU.join.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\nb 2\n' >f1 && printf 'a x\nb y\n' >f2 || pc_fail "cannot create input"
printf 'left over\n' >rest || pc_fail "cannot create input"
pc_test
got=$(join - f2 <f1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file1 did not read standard input"
got=$(join f1 - <f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file2 did not read standard input"
got=$(cat f1 | join - f2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file1 did not read a pipe"
got=$(cat f2 | join -a 1 f1 - 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}rc=0" ] || pc_fail "- as file2 did not read a pipe"
got=$( (join f1 f2; cat) <rest 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}b 2 y${NL}left over${NL}rc=0" ] || pc_fail "standard input was read although no operand was -"
pc_pass
