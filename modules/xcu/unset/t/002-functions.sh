# unset -f name removes a function, so the name is then not found (status
# 127), and leaves a variable of the same name alone; unset -v leaves the
# function alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.002
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.002
pc_test
pcf() { echo fn; }
unset -f pcf
(pcf) 2>/dev/null
rc=$?
[ "$rc" -eq 127 ] || pc_fail "calling the removed function gave $rc, expected 127"
pcf() { echo fn; }
pcf=var
unset -f pcf
[ "$pcf" = var ] || pc_fail "unset -f removed the variable"
pcf() { echo fn; }
unset -v pcf
[ -z "${pcf+set}" ] || pc_fail "unset -v left the variable"
r=$(pcf)
[ "$r" = fn ] || pc_fail "unset -v removed the function"
pcg() { echo g; }
pch() { echo h; }
unset -f pcg pch
r=$( (pcg; pch) 2>/dev/null )
[ -z "$r" ] || pc_fail "one of two functions survived unset -f: '$r'"
pc_pass
