# Text utilities process input lines of 2047 bytes plus the newline unchanged: cat, sort, sed, awk, cut, head, tail, uniq, grep, tee, paste and wc
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
awk 'BEGIN {
	split("a b c", ch, " ")
	for (k = 1; k <= 3; k++) {
		s = ""
		for (i = 0; i < 2047; i++) s = s ch[k]
		print s
	}
}' >in || pc_unresolved "cannot build the input"
[ "$(wc -c <in | tr -d ' ')" = 6144 ] || pc_unresolved "input has an unexpected size"
pc_test
for c in "cat in" "sort in" "sed -n p in" "awk '{ print }' in" "cut -c 1-2047 in" "head -n 3 in" "tail -n 3 in" "uniq in" "grep -v zzz in" "tee tee.out <in" "paste in" "cat <in"; do
	eval "$c" >out 2>err </dev/null || pc_fail "'$c' failed on long lines: $(dd bs=100 count=1 <err 2>/dev/null)"
	cmp -s in out || pc_fail "'$c' changed the long lines"
done
n=$(wc -l <in | tr -d ' ')
[ "$n" = 3 ] || pc_fail "wc -l counted $n lines"
pc_pass
