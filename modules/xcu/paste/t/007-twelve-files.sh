# paste accepts at least twelve file operands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.007
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.007
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'x\n' >f1 || pc_fail "cannot create input"
set --
exp=x
i=1
while [ $i -le 12 ]; do
	set -- "$@" f1
	[ $i -gt 1 ] && exp=$exp${T}x
	i=$((i + 1))
done
pc_test
got=$(paste "$@" 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${exp}${NL}rc=0" ] || pc_fail "twelve file operands were not all joined"
pc_pass
