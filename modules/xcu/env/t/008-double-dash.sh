# -- ends the options of env.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.008
. "$PC_CORE/pc.sh"
pc_begin XCU.env.008
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
pc_test
env -- PCENV_D=1 "$shp" -c 'echo "$PCENV_D"' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env -- exited $rc"
got=$(cat out)
[ "$got" = 1 ] || pc_fail "the utility saw '$got', wanted 1"
env -- "$shp" -c 'echo hi' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = hi ] || pc_fail "env -- sh -c wrote '$got', wanted hi"
env -i -- PCENV_D=2 "$shp" -c 'echo "$PCENV_D"' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = 2 ] || pc_fail "env -i -- saw '$got', wanted 2"
pc_pass
