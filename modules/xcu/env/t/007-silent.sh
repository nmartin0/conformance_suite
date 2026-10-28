# env writes nothing itself when it runs a utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.007
. "$PC_CORE/pc.sh"
pc_begin XCU.env.007
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
pc_test
env PCENV_Q=1 true </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env PCENV_Q=1 true exited $rc"
[ ! -s out ] || pc_fail "env wrote to standard output: $(cat out)"
env -i PCENV_Q=1 "$shp" -c ':' </dev/null >out 2>/dev/null
[ ! -s out ] || pc_fail "env -i wrote to standard output: $(cat out)"
pc_pass
