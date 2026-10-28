# The command name is the first field produced by expanding the words in
# order: a word that expands to nothing is skipped, and with no command name
# left the assignments apply to the current environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.006
pc_test
empty=
x=
y=
r=$($empty echo hi)
[ "$r" = hi ] || pc_fail "an empty first word: got '$r'"
r=$($x $y echo z)
[ "$r" = z ] || pc_fail "two empty words: got '$r'"
c=echo
r=$($c a b)
[ "$r" = "a b" ] || pc_fail "name from a variable: got '$r'"
unset PCV
PCV=5 $empty
[ "$PCV" = 5 ] || pc_fail "assignment with an all-empty command: PCV='$PCV'"
pc_pass
