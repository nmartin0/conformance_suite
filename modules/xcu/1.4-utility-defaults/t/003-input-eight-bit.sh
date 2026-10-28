# Utilities process input files containing every byte value except NUL and newline without altering them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.003
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=1
while [ "$i" -le 255 ]; do
	[ "$i" -eq 10 ] || printf '%b' "\\0$(printf '%03o' "$i")"
	i=$((i + 1))
done >in || pc_unresolved "cannot write the input"
printf '\n' >>in || pc_unresolved "cannot write the input"
[ "$(wc -c <in | tr -d ' ')" = 255 ] || pc_unresolved "the input has the wrong size"
pc_test
chk() {
	"$@" <in >o 2>e
	cmp -s o in || pc_fail "$* changed the bytes"
}
chk cat
chk cat in
chk sort
chk sort in
chk head -n 1
chk tail -n 1
chk uniq
chk cut -b 1-1000
chk fold -b -w 1000
chk sed -n p
tee t.out <in >o 2>e
cmp -s o in && cmp -s t.out in || pc_fail "tee changed the bytes"
pc_pass
