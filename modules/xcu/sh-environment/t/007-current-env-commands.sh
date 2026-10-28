# Compound commands, function bodies, eval and dot run in the current shell
# environment, so their assignments stay.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.007
printf '%s\n' 'PCV=8' >pc.dd || pc_unresolved "write"
unset PCV PCV4
pc_test
{ PCV=1; }
[ "$PCV" = 1 ] || pc_fail "brace group"
if true; then PCV=2; fi
[ "$PCV" = 2 ] || pc_fail "if"
for i in 1; do PCV=3; done
[ "$PCV" = 3 ] || pc_fail "for"
while [ -z "${PCV4+set}" ]; do PCV4=1; done
[ "$PCV4" = 1 ] || pc_fail "while"
case x in x) PCV=5 ;; esac
[ "$PCV" = 5 ] || pc_fail "case"
pcf() { PCV=6; }
pcf
[ "$PCV" = 6 ] || pc_fail "function"
eval 'PCV=7'
[ "$PCV" = 7 ] || pc_fail "eval"
. ./pc.dd
[ "$PCV" = 8 ] || pc_fail "dot"
pc_pass
