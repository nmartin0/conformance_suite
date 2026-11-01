# nm -v ordering
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.006
# By default the output is sorted by symbol name (POSIX locale); with -v the
# defined symbols appear in nondecreasing order of value (compared as
# numbers).
# MUTANT    nm.default_by_value
# MUTANT    nm.default_name_desc
# MUTANT    nm.v_ignored
# MUTANT    nm.v_text_order
# MUTANT    nm.v_descending
# MUTANT    nm.v_by_size
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.v_ties_reversed
# VARIANT   nm.v_undef_first
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
{
	echo 'int pcdata[4] = {1, 2, 3, 4};'
	echo 'int pcbss[8];'
	echo 'static int pclocal = 5;'
	echo 'extern int pcundef(void);'
	i=40
	while [ "$i" -ge 1 ]; do
		printf 'int pcf%02d(int x) { return x * %d + 1; }\n' "$i" "$i"
		i=$((i - 1))
	done
	echo 'int pcfunc(void) { return pclocal + pcundef() + pcdata[0] + pcbss[0]; }'
} >x.c
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
pc_test
nm -P x.o >o 2>err </dev/null || pc_fail "nm -P: nonzero status"
awk '{ print $1 }' o >names
[ "$(wc -l <names)" -gt 40 ] || pc_fail "nm -P listed too few symbols"
sort -c names 2>/dev/null || pc_fail "the default output is not sorted by symbol name"
names=$(awk '$1 ~ /pcf[0-9][0-9]$/ { print substr($1, length($1) - 4) }' o | tr '\n' ' ')
want=$(i=1; while [ "$i" -le 40 ]; do printf 'pcf%02d ' "$i"; i=$((i + 1)); done)
[ "$names" = "$want" ] || pc_fail "default order of the functions is not by name"
nm -vP -t d x.o >o2 2>err </dev/null || pc_fail "nm -vP -t d: nonzero status"
[ "$(wc -l <o2)" -eq "$(wc -l <o)" ] || pc_fail "-v changed the number of lines"
# the defined symbols: values, as decimal numbers, never decrease
awk '$2 != "U" { if ($3 !~ /^[0-9]+$/) bad = 1; else { if (n > 0 && $3 + 0 < prev) bad = 1; prev = $3 + 0; n++ } } END { exit (bad || n < 40) }' o2 || pc_fail "-v did not give nondecreasing numeric values for the defined symbols"
# the functions: lowest value first, which is the reverse of name order
names=$(awk '$1 ~ /pcf[0-9][0-9]$/ { print substr($1, length($1) - 4) }' o2 | tr '\n' ' ')
want=$(i=40; while [ "$i" -ge 1 ]; do printf 'pcf%02d ' "$i"; i=$((i - 1)); done)
[ "$names" = "$want" ] || pc_fail "with -v the functions are not in the order of their values"
pc_pass
