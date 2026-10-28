# Assignments before a regular utility or built-in are exported to that
# command only: the utility sees them and the shell's own value is unchanged
# afterwards.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.002
pc_test
unset PCV
r=$(PCV=tmp ${PC_SH:-sh} -c 'printf %s "${PCV-unset}"')
[ "$r" = tmp ] || pc_fail "the utility saw '$r', expected tmp"
[ -z "${PCV+set}" ] || pc_fail "the assignment stayed in the shell (PCV='$PCV')"
PCV=old
PCV=tmp ${PC_SH:-sh} -c ':'
[ "$PCV" = old ] || pc_fail "PCV changed to '$PCV'"
PCV=other true
[ "$PCV" = old ] || pc_fail "assignment before true persisted (PCV='$PCV')"
pc_pass
