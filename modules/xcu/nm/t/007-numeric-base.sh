# nm -t d, o, x
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.007
# With -P, the value and size fields are made of decimal digits for -t d,
# octal digits for -t o and hexadecimal digits for -t x (and -P alone is as
# -t x): the same numbers in the three bases, written as plain digits with no
# prefix, padding or capital letters, also for a size above 32 bits; -tx
# (argument attached) is as -t x.
# MUTANT    nm.t_d_as_hex
# MUTANT    nm.t_x_dec
# MUTANT    nm.t_o_dec
# MUTANT    nm.t_o_as_hex
# MUTANT    nm.t_ignored
# MUTANT    nm.t_pdefault_dec
# MUTANT    nm.t_x_upper
# MUTANT    nm.t_x_0x
# MUTANT    nm.t_o_zero
# MUTANT    nm.t_size_dec
# MUTANT    nm.t_value_dec
# MUTANT    nm.size_trunc32
# MUTANT    nm.value_size_swapped
# MUTANT    nm.t_attached_bad
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.size_zero
# VARIANT   nm.undef_zero
# VARIANT   nm.tabbed
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
char pcdist[1237] = {1};
int pcbss[8];
static int pclocal = 5;
extern int pcundef(void);
int pcz1(void) { return 1; }
int pcy2(void) { return 2; }
int pcx3(void) { return 3; }
int pcw4(void) { return 4; }
int pcv5(void) { return 5; }
int pcfunc(void) { return pclocal + pcundef() + pcdata[0] + pcbss[0] + pcdist[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
# an object whose size does not fit in 32 bits (left out where the compiler cannot make one)
printf 'char pchuge[4294967301ull];\n' >big.c
huge=
if c17 -c big.c </dev/null >/dev/null 2>&1 && [ -f big.o ]; then huge=big.o; fi
pc_test
for obj in x.o $huge; do
	nm -P -t d $obj >od 2>err </dev/null || pc_fail "nm -P -t d $obj: nonzero status"
	nm -P -t o $obj >oo 2>err </dev/null || pc_fail "nm -P -t o $obj: nonzero status"
	nm -P -t x $obj >ox 2>err </dev/null || pc_fail "nm -P -t x $obj: nonzero status"
	nm -P $obj >op 2>err </dev/null || pc_fail "nm -P $obj: nonzero status"
	nm -P -tx $obj >oa 2>err </dev/null || pc_fail "nm -P -tx $obj: nonzero status"
	for f in od oo ox op oa; do
		[ -s $f ] || pc_fail "nm -P wrote nothing ($f of $obj)"
	done
	cmp -s ox op || pc_fail "-P alone is not as -t x ($obj)"
	cmp -s ox oa || pc_fail "-tx is not as -t x ($obj)"
	awk '$2 != "U" && NF >= 3 && ($3 !~ /^(0|[1-9][0-9]*)$/ || (NF >= 4 && $4 !~ /^(0|[1-9][0-9]*)$/)) { bad = 1 } END { exit bad }' od || pc_fail "-t d gave a field that is not a plain decimal number ($obj)"
	awk '$2 != "U" && NF >= 3 && ($3 !~ /^(0|[1-7][0-7]*)$/ || (NF >= 4 && $4 !~ /^(0|[1-7][0-7]*)$/)) { bad = 1 } END { exit bad }' oo || pc_fail "-t o gave a field that is not a plain octal number ($obj)"
	awk '$2 != "U" && NF >= 3 && ($3 !~ /^(0|[1-9a-f][0-9a-f]*)$/ || (NF >= 4 && $4 !~ /^(0|[1-9a-f][0-9a-f]*)$/)) { bad = 1 } END { exit bad }' ox || pc_fail "-t x gave a field that is not a plain lower-case hexadecimal number ($obj)"
	# the same lines, with the same numbers in each base
	nl=$(wc -l <od)
	[ "$(wc -l <oo)" -eq "$nl" ] && [ "$(wc -l <ox)" -eq "$nl" ] || pc_fail "the three bases give different numbers of lines ($obj)"
	i=1
	while [ "$i" -le "$nl" ]; do
		set -- $(sed -n "${i}p" od)
		dn=$1 dt=$2 dv=${3:-} ds=${4:-}
		set -- $(sed -n "${i}p" oo)
		on=$1 ot=$2 ov=${3:-} os=${4:-}
		set -- $(sed -n "${i}p" ox)
		xn=$1 xt=$2 xv=${3:-} xs=${4:-}
		[ "$dn" = "$on" ] && [ "$dn" = "$xn" ] && [ "$dt" = "$ot" ] && [ "$dt" = "$xt" ] || pc_fail "line $i differs in name or type between the bases ($obj)"
		if [ -n "$dv" ] && [ "$dt" != U ]; then
			[ "$ov" = "$(printf '%o' "$dv")" ] || pc_fail "$dn: value $dv in decimal is '$ov' in octal ($obj)"
			[ "$xv" = "$(printf '%x' "$dv")" ] || pc_fail "$dn: value $dv in decimal is '$xv' in hexadecimal ($obj)"
		fi
		if [ -n "$ds" ] && [ "$dt" != U ]; then
			[ "$os" = "$(printf '%o' "$ds")" ] || pc_fail "$dn: size $ds in decimal is '$os' in octal ($obj)"
			[ "$xs" = "$(printf '%x' "$ds")" ] || pc_fail "$dn: size $ds in decimal is '$xs' in hexadecimal ($obj)"
		fi
		i=$((i + 1))
	done
done
# the objects of known size: where the size is reported, it is in the size place
nm -P -t d x.o >od 2>/dev/null </dev/null
awk '$1 ~ /pcdist$/ && NF >= 4 && $4 != 1237 && $3 == 1237 { bad = 1 } END { exit bad }' od || pc_fail "the size of the 1237-byte object is in the value place"
awk '$1 ~ /pcdata$/ && NF >= 4 && $4 != 16 && $3 == 16 { bad = 1 } END { exit bad }' od || pc_fail "the size of the 16-byte object is in the value place"
if [ -n "$huge" ]; then
	nm -P -t d big.o >od 2>/dev/null </dev/null
	awk '$1 ~ /pchuge$/ && NF >= 4 && $3 != 4294967301 && $4 != 4294967301 && ($3 == 5 || $4 == 5) { bad = 1 } END { exit bad }' od || pc_fail "the size 4294967301 is shown cut to 32 bits"
fi
pc_pass
