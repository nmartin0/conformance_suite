# Arguments reach the utility unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.006
. "$PC_CORE/pc.sh"
pc_begin XCU.env.006
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
pc_test
env "$shp" -c 'printf "%s|" "$@"' sh a 'b c' '' -x -- '*' '$HOME' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env exited $rc"
got=$(cat out)
[ "$got" = 'a|b c||-x|--|*|$HOME|' ] || pc_fail "the utility saw '$got'"
(exec env "$shp" -c 'printf "%s|" "$@"' sh 1 '2 3') </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = '1|2 3|' ] || pc_fail "exec env: the utility saw '$got'"
pc_pass
