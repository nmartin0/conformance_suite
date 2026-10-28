# Assignments before a special built-in affect the current environment and
# stay after it finishes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.003
pc_test
unset PCV PCW
PCV=2 :
[ "$PCV" = 2 ] || pc_fail "after colon PCV='$PCV'"
PCW=3 eval true
[ "$PCW" = 3 ] || pc_fail "after eval PCW='$PCW'"
pc_pass
