# nohup runs its utility with SIGHUP ignored.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nohup.001
. "$PC_CORE/pc.sh"
pc_begin XCU.nohup.001
nl='
'
shp=$(command -v "${PC_SH:-sh}")
case $shp in
/*) ;;
*) pc_unresolved "setup: cannot find an absolute path for sh ('$shp')" ;;
esac
"$shp" -c 'kill -s HUP $$; echo alive' </dev/null >plain.out 2>/dev/null
got=$(cat plain.out)
[ "$got" != alive ] || pc_unresolved "setup: a plain shell survives SIGHUP, so SIGHUP is already ignored here"
pc_test
nohup "$shp" -c 'kill -s HUP $$; echo alive' </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = alive ] || pc_fail "the shell run by nohup did not survive SIGHUP (it wrote '$got')"
(exec nohup "$shp" -c 'kill -s HUP $$; echo alive2') </dev/null >out 2>/dev/null
got=$(cat out)
[ "$got" = alive2 ] || pc_fail "the shell run by exec nohup did not survive SIGHUP (it wrote '$got')"
pc_pass
