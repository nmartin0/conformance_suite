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
# the utility named is the one that runs, and env adds no output of its own
env PCENV_Q=1 "$shp" -c 'printf "%s\n" "$PCENV_Q"' </dev/null >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "env PCENV_Q=1 sh -c ... exited $rc"
got=
more=
extra=no
{
	IFS= read -r got
	if IFS= read -r more; then extra=yes; fi
} <out
[ "$got" = 1 ] || pc_fail "the utility run by env wrote '$got', expected 1"
[ "$extra" = no ] && [ -z "$more" ] || pc_fail "env wrote more than the utility did"
pc_pass
