# A failing utility does not stop xargs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.013
# MUTANT    xargs.fail_stops
# MUTANT    xargs.fail_status0
# MUTANT    xargs.fail_status126
# MUTANT    xargs.fail_last_only
# VARIANT   xargs.status_first
# VARIANT   xargs.status_max
# VARIANT   xargs.status_last
# VARIANT   xargs.status_sum
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.013
LC_ALL=C; export LC_ALL
d=pcxargs013.$$
trap 'rm -rf "$d"' EXIT
mkdir "$d" || pc_fail "cannot make $d"
pc_test
# a exits 3, b exits 7, c succeeds: the last run is not the only one that counts
printf 'a b c\n' | xargs -n 1 "${PC_SH:-sh}" -c 'echo "$1" >>"$0"; case $1 in a) exit 3 ;; b) exit 7 ;; esac' "$d/log" >out 2>err
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "xargs exited with status $rc, expected 1-125"
n=0 fa=no fb=no fc=no
while IFS= read -r l; do
	n=$((n + 1))
	[ "$l" = a ] && fa=yes
	[ "$l" = b ] && fb=yes
	[ "$l" = c ] && fc=yes
done <"$d/log"
[ "$fa" = yes ] && [ "$fb" = yes ] && [ "$fc" = yes ] && [ "$n" -eq 3 ] || pc_fail "the utility did not run once for each of a, b and c ($n runs logged)"
pc_pass
