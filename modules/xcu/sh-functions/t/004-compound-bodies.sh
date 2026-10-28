# The body of a function may be any compound command: a subshell, if, for,
# case or while.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.004
pc_test
PCV=out
pcf() ( PCV=in; echo sub )
r=$(pcf)
[ "$r" = sub ] && [ "$PCV" = out ] || pc_fail "subshell body: r='$r' PCV='$PCV'"
pcf() if true; then echo yes; fi
r=$(pcf)
[ "$r" = yes ] || pc_fail "if body: '$r'"
pcf() for i in 1 2; do printf %s "$i"; done
r=$(pcf)
[ "$r" = 12 ] || pc_fail "for body: '$r'"
pcf() case $1 in x) echo cx ;; *) echo other ;; esac
r=$(pcf x)
[ "$r" = cx ] || pc_fail "case body: '$r'"
pcf() while [ -z "$PCW" ]; do PCW=1; echo once; done
r=$(pcf)
[ "$r" = once ] || pc_fail "while body: '$r'"
pc_pass
