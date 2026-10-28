# A PATH operand is used to find the utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.env.005
. "$PC_CORE/pc.sh"
pc_begin XCU.env.005
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
mkdir d || pc_unresolved "setup: cannot create a directory"
ln -s "$shp" d/pcsh || pc_unresolved "setup: cannot create a symbolic link"
command -v pcsh >/dev/null 2>&1 && pc_unresolved "setup: pcsh is already on PATH"
here=$PWD
pc_test
env PATH="$here/d" pcsh -c 'echo ok' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env PATH=... pcsh exited $rc"
got=$(cat out)
[ "$got" = ok ] || pc_fail "the utility wrote '$got', wanted ok"
(exec env PATH="$here/d" pcsh -c 'echo ok2') </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = ok2 ] || pc_fail "exec env PATH=... pcsh wrote '$got', wanted ok2"
pc_pass
