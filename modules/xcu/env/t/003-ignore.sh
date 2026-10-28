# env -i starts from an empty environment.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.003
. "$PC_CORE/pc.sh"
pc_begin XCU.env.003
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
PCENV_INH=inherited
export PCENV_INH
pc_test
env -i </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env -i exited $rc"
[ ! -s out ] || pc_fail "env -i with no operand wrote something: $(cat out)"
env -i PCENV_X=1 </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = PCENV_X=1 ] || pc_fail "env -i PCENV_X=1 wrote '$got', wanted just PCENV_X=1"
env -i PCENV_X=1 "$shp" -c 'echo "${PCENV_INH+set}:$PCENV_X"' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = ":1" ] || pc_fail "the utility saw '$got', wanted ':1'"
(exec env -i PCENV_X=2) </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = PCENV_X=2 ] || pc_fail "exec env -i PCENV_X=2 wrote '$got'"
pc_pass
