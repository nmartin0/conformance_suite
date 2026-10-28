# { list; } runs the list in the current environment and its status is that
# of the list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.002
unset PCV PCW
pc_test
{ PCV=1; PCW=2; }
[ "$PCV$PCW" = 12 ] || pc_fail "assignments missing: PCV='$PCV' PCW='$PCW'"
{ false; (exit 6); }
[ "$?" -eq 6 ] || pc_fail "status of the group was $?"
{ (exit 6); true; }
[ "$?" -eq 0 ] || pc_fail "status of { (exit 6); true; } was $?"
pc_pass
