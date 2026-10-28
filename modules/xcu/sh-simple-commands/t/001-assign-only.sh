# A command made only of variable assignments sets the variables in the
# current shell environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.001
pc_test
unset PCV PCW
PCV=1
[ "$PCV" = 1 ] || pc_fail "PCV='$PCV'"
PCV=2 PCW=3
[ "$PCV$PCW" = 23 ] || pc_fail "PCV='$PCV' PCW='$PCW'"
pc_pass
