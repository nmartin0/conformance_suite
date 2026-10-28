# strings -a scans the whole file, however far a string is from the start.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strings.004
. "$PC_CORE/pc.sh"
pc_begin XCU.strings.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000' >chunk || pc_fail "cannot create input"
i=0
while [ $i -lt 11 ]; do
	cat chunk chunk >chunk2 && mv chunk2 chunk || pc_fail "cannot create input"
	i=$((i + 1))
done
{ cat chunk; printf 'tailstring\n'; } >big || pc_fail "cannot create input"
pc_test
got=$(strings -a big 2>/dev/null </dev/null | while IFS= read -r l; do printf '[%s]' "$l"; done)
case $got in
*"[tailstring]"*) ;;
*) pc_fail "the string after 131072 NUL bytes was not found" ;;
esac
pc_pass
