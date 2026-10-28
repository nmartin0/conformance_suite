# A failing utility does not stop xargs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.013
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.013
LC_ALL=C; export LC_ALL
d=pcxargs013.$$
trap 'rm -rf "$d"' EXIT
mkdir "$d" || pc_fail "cannot make $d"
pc_test
printf 'a b\n' | xargs -n 1 "${PC_SH:-sh}" -c 'echo "$1" >>"$0"; exit 1' "$d/log" >out 2>err
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "xargs exited with status $rc, expected 1-125"
n=0 fa=no fb=no
while IFS= read -r l; do
	n=$((n + 1))
	[ "$l" = a ] && fa=yes
	[ "$l" = b ] && fb=yes
done <"$d/log"
[ "$fa" = yes ] && [ "$fb" = yes ] && [ "$n" -eq 2 ] || pc_fail "the utility did not run once for each of a and b ($n runs logged)"
pc_pass
