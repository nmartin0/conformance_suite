# Without -s, paste writes nothing to standard output when a file cannot be opened; with -s it carries on with the other files; both give a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.008
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.008
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'x\ny\n' >good || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
paste nosuchfile good >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although a file could not be opened"
[ -s out ] && pc_fail "output was written although a file could not be opened"
got=$(paste -s nosuchfile good 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
*"x${T}y${NL}rc="[1-9]*) ;;
*) pc_fail "-s did not process the readable file or gave status 0" ;;
esac
pc_pass
