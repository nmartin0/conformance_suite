# name= sets the variable to the empty string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.009
. "$PC_CORE/pc.sh"
pc_begin XCU.env.009
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
PCENV_E=full
export PCENV_E
pc_test
env PCENV_E= "$shp" -c 'echo "${PCENV_E-unset}:"' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = ":" ] || pc_fail "an inherited variable set to empty by env reads '$got', wanted ':'"
env PCENV_N= "$shp" -c 'echo "${PCENV_N-unset}:"' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = ":" ] || pc_fail "a new variable set to empty by env reads '$got', wanted ':'"
pc_pass
