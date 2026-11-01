# nm -x and -o
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.008
# "nm -x x.o" prints the same as "nm -t x x.o" and "nm -o x.o" the same as
# "nm -t o x.o"; with -P too.
# MUTANT    nm.x_dec
# MUTANT    nm.x_as_o
# MUTANT    nm.o_dec
# MUTANT    nm.o_as_x
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
{
	i=1
	while [ "$i" -le 12 ]; do
		printf 'int pcf%02d(int x) { return x * %d + 1; }\n' "$i" "$i"
		i=$((i + 1))
	done
	echo 'int pcdata[40] = {1, 2, 3, 4};'
} >x.c
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
# the three bases must give different text, otherwise -x and -o cannot be told apart
nm -P -t d x.o >pd 2>/dev/null </dev/null || pc_unresolved "setup: nm -P -t d failed"
nm -P -t o x.o >po 2>/dev/null </dev/null || pc_unresolved "setup: nm -P -t o failed"
nm -P -t x x.o >px 2>/dev/null </dev/null || pc_unresolved "setup: nm -P -t x failed"
if cmp -s pd px || cmp -s pd po || cmp -s po px; then
	pc_unresolved "setup: the bases are not told apart in the output"
fi
pc_test
nm -x x.o >o1 2>err </dev/null || pc_fail "nm -x: nonzero status"
nm -t x x.o >o2 2>err </dev/null || pc_fail "nm -t x: nonzero status"
[ -s o1 ] || pc_fail "nm -x wrote nothing"
cmp -s o1 o2 || pc_fail "-x differs from -t x"
nm -o x.o >o3 2>err </dev/null || pc_fail "nm -o: nonzero status"
nm -t o x.o >o4 2>err </dev/null || pc_fail "nm -t o: nonzero status"
[ -s o3 ] || pc_fail "nm -o wrote nothing"
cmp -s o3 o4 || pc_fail "-o differs from -t o"
nm -xP x.o >o5 2>err </dev/null || pc_fail "nm -xP: nonzero status"
nm -P -t x x.o >o5r 2>err </dev/null || pc_fail "nm -P -t x: nonzero status"
cmp -s o5 o5r || pc_fail "-xP differs from -P -t x"
nm -oP x.o >o6 2>err </dev/null || pc_fail "nm -oP: nonzero status"
nm -P -t o x.o >o6r 2>err </dev/null || pc_fail "nm -P -t o: nonzero status"
cmp -s o6 o6r || pc_fail "-oP differs from -P -t o"
pc_pass
