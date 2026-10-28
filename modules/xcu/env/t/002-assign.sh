# name=value operands set the utility's environment only.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.002
. "$PC_CORE/pc.sh"
pc_begin XCU.env.002
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
PCENV_A=old
export PCENV_A
unset PCENV_B
pc_test
env PCENV_A=new PCENV_B=two "$shp" -c 'echo "$PCENV_A $PCENV_B"' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env exited $rc"
got=$(cat out)
[ "$got" = "new two" ] || pc_fail "the utility saw '$got', wanted 'new two'"
[ "$PCENV_A" = old ] || pc_fail "PCENV_A is '$PCENV_A' in the caller after env, wanted old"
[ -z "${PCENV_B+set}" ] || pc_fail "PCENV_B is set in the caller after env"
(exec env PCENV_A=newer "$shp" -c 'echo "$PCENV_A"') </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = newer ] || pc_fail "exec env: the utility saw '$got', wanted 'newer'"
pc_pass
