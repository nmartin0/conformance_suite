# uuencode historical line structure
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.003
# Without -m, 50 input bytes of ordinary text give data lines each starting
# with a length character that stands for 1 to 45 bytes and followed by
# exactly the four-for-three encoding of that many bytes; the byte counts
# add up to 50.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '%s' 'The quick brown fox jumps over the lazy dog 123456' >in || pc_unresolved "setup: cannot write input"
sz=$(wc -c <in); sz=$((sz + 0))
[ "$sz" -eq 50 ] || pc_unresolved "setup: input is $sz bytes"
pc_test
uuencode dest.bin <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
sed '1d;$d' out | sed '$d' >data
total=0
nl=0
while IFS= read -r l; do
	nl=$((nl + 1))
	first=${l%"${l#?}"}
	rest=${l#?}
	n=$(($(printf '%d' "'$first") - 32))
	{ [ "$n" -ge 1 ] && [ "$n" -le 45 ]; } || pc_fail "length character of line $nl stands for $n bytes"
	want=$(((n + 2) / 3 * 4))
	[ "${#rest}" -eq "$want" ] || pc_fail "line $nl has ${#rest} encoded characters for $n bytes, expected $want"
	total=$((total + n))
done <data
[ "$total" -eq 50 ] || pc_fail "data lines stand for $total bytes, expected 50"
pc_pass
