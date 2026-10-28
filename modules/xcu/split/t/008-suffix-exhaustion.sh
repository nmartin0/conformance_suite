# When the suffixes run out split fails after writing the last file with a valid suffix and keeps the files it made.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.008
. "$PC_CORE/pc.sh"
pc_begin XCU.split.008
LC_ALL=POSIX
export LC_ALL
NL='
'
i=1
while [ $i -le 27 ]; do
	printf 'line%s\n' "$i"
	i=$((i + 1))
done >in || pc_fail "cannot create input"
pc_test
split -a 1 -l 1 in >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although the suffixes were used up"
got=$(cat xa; printf x)
[ "$got" = "line1${NL}x" ] || pc_fail "file xa is missing or wrong"
got=$(cat xz; printf x)
[ "$got" = "line26${NL}x" ] || pc_fail "file xz (the 26th) is missing or wrong"
pc_pass
