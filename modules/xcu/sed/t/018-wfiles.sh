# r and w file names, ten w files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.018
# The r and w commands and the w flag of s take a file name separated by
# one or more blanks, and a script can use at least ten different w files.
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.018
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'line\n' >in || pc_fail "cannot create input"
printf 'from-rfile\n' >rf || pc_fail "cannot create input"
script=
i=1
while [ "$i" -le 10 ]; do
	script="$script
w   out$i"
	i=$((i + 1))
done
pc_test
sed -n "$script" in >/dev/null 2>&1 </dev/null || pc_fail "sed with ten w files failed"
i=1
while [ "$i" -le 10 ]; do
	[ "$(cat out$i 2>/dev/null)" = "line" ] || pc_fail "out$i did not get the line"
	i=$((i + 1))
done
got=$(sed -e 'r   rf' in 2>/dev/null </dev/null)
[ "$got" = "line${NL}from-rfile" ] || pc_fail "r with several blanks: $got"
sed -n 's/line/LINE/w    outs' in >/dev/null 2>&1 </dev/null || pc_fail "s///w with several blanks failed"
[ "$(cat outs 2>/dev/null)" = "LINE" ] || pc_fail "the w flag with several blanks did not write the file"
pc_pass
