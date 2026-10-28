# nohup keeps a non-terminal standard output and makes no nohup.out.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nohup.002
. "$PC_CORE/pc.sh"
pc_begin XCU.nohup.002
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
home=${HOME:-/pc-no-such-home}
pre=
[ -e "$home/nohup.out" ] && pre=1
pc_test
nohup "$shp" -c 'echo "$1 $2"' sh a 'b c' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nohup exited $rc"
got=$(cat out)
[ "$got" = "a b c" ] || pc_fail "the utility's output was '$got', wanted 'a b c'"
[ ! -e nohup.out ] || pc_fail "nohup.out was created in the current directory"
if [ -z "$pre" ]; then
	[ ! -e "$home/nohup.out" ] || pc_fail "nohup.out was created in the home directory"
fi
pc_pass
