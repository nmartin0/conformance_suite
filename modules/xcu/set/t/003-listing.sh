# set with no options or operands lists the shell variables as name=value in
# the collation order of the current locale, with values quoted so that the
# lines can be read back.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.003
. "$PC_CORE/pc.sh"
pc_begin XCU.set.003
LC_ALL=POSIX
export LC_ALL
PCSET_B='second one'
PCSET_A="it's \"q\" \$x \\ back"
wa=$PCSET_A
wb=$PCSET_B
pc_test
set | grep '^PCSET_[AB]=' >pc.set
first=$(sed -n 1p pc.set)
second=$(sed -n 2p pc.set)
case $first in
PCSET_A=*) ;;
*) pc_fail "first listed line is '$first', expected the PCSET_A line" ;;
esac
case $second in
PCSET_B=*) ;;
*) pc_fail "second listed line is '$second', expected the PCSET_B line" ;;
esac
PCSET_A=changed
PCSET_B=changed
eval "$first"
eval "$second"
[ "$PCSET_A" = "$wa" ] || pc_fail "line for PCSET_A did not read back (got '$PCSET_A')"
[ "$PCSET_B" = "$wb" ] || pc_fail "line for PCSET_B did not read back (got '$PCSET_B')"
pc_pass
