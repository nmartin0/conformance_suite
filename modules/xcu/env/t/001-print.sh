# env with no operand writes the environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.001
. "$PC_CORE/pc.sh"
pc_begin XCU.env.001
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
PCENV_T1='some value'
export PCENV_T1
pc_test
env </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env exited $rc"
found=
while IFS= read -r line; do
	[ "$line" = "PCENV_T1=some value" ] && found=1
done <out
[ -n "$found" ] || pc_fail "the line PCENV_T1=some value was not written"
(exec env) </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exec env exited $rc"
found=
while IFS= read -r line; do
	[ "$line" = "PCENV_T1=some value" ] && found=1
done <out
[ -n "$found" ] || pc_fail "exec env did not write the line PCENV_T1=some value"
pc_pass
