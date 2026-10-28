# nm -t d, o, x
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.007
# With -P, the value and size fields are made of decimal digits for -t d,
# octal digits for -t o and hexadecimal digits for -t x (and -P alone is as
# -t x).
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >x.c <<'EOC'
int pcdata[4] = {1, 2, 3, 4};
int pcbss[8];
static int pclocal = 5;
extern int pcundef(void);
int pcz1(void) { return 1; }
int pcy2(void) { return 2; }
int pcx3(void) { return 3; }
int pcw4(void) { return 4; }
int pcfunc(void) { return pclocal + pcundef() + pcdata[0] + pcbss[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
pc_test
nm -P -t d x.o >od 2>err </dev/null || pc_fail "nm -P -t d: nonzero status"
nm -P -t o x.o >oo 2>err </dev/null || pc_fail "nm -P -t o: nonzero status"
nm -P -t x x.o >ox 2>err </dev/null || pc_fail "nm -P -t x: nonzero status"
nm -P x.o >op 2>err </dev/null || pc_fail "nm -P: nonzero status"
awk '$2 != "U" && NF >= 3 && ($3 !~ /^[0-9]+$/ || (NF >= 4 && $4 !~ /^[0-9]+$/)) { bad = 1 } END { exit bad }' od || pc_fail "-t d gave a non-decimal field"
awk '$2 != "U" && NF >= 3 && ($3 !~ /^[0-7]+$/ || (NF >= 4 && $4 !~ /^[0-7]+$/)) { bad = 1 } END { exit bad }' oo || pc_fail "-t o gave a non-octal field"
awk '$2 != "U" && NF >= 3 && ($3 !~ /^[0-9a-fA-F]+$/ || (NF >= 4 && $4 !~ /^[0-9a-fA-F]+$/)) { bad = 1 } END { exit bad }' ox || pc_fail "-t x gave a non-hexadecimal field"
sd=$(awk '$1 ~ /pcdata$/ { print $4 }' od)
if [ "$sd" != 16 ]; then
	pc_unresolved "the size of the 16-byte object is not reported"
fi
so=$(awk '$1 ~ /pcdata$/ { print $4 }' oo)
sx=$(awk '$1 ~ /pcdata$/ { print $4 }' ox)
sp=$(awk '$1 ~ /pcdata$/ { print $4 }' op)
[ "$so" = 20 ] || pc_fail "-t o gives size '$so' for 16 bytes, expected 20"
[ "$sx" = 10 ] || pc_fail "-t x gives size '$sx' for 16 bytes, expected 10"
[ "$sp" = 10 ] || pc_fail "-P alone gives size '$sp' for 16 bytes, expected 10 (as -t x)"
pc_pass
