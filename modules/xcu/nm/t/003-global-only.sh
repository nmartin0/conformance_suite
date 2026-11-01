# nm -g
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.003
# "nm -g x.o" lists global symbols and not a static (local) variable or
# function.
# MUTANT    nm.g_ignored
# MUTANT    nm.g_inverted
# MUTANT    nm.g_adds_local_data
# MUTANT    nm.g_adds_local_text
# MUTANT    nm.g_drops_data
# MUTANT    nm.g_drops_bss
# MUTANT    nm.g_drops_text
# MUTANT    nm.g_first_only
# MUTANT    nm.g_none
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.common_C
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.003
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
static int pcstat(void) { return 6; }
extern int pcundef(void);
int pcz1(void) { return 1; }
int pcy2(void) { return 2; }
int pcx3(void) { return 3; }
int pcw4(void) { return 4; }
int pcfunc(void) { return pclocal + pcstat() + pcundef() + pcdata[0] + pcbss[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
nm -P x.o >all 2>/dev/null </dev/null || pc_unresolved "setup: nm -P failed"
for s in pclocal pcstat; do
	n=$(awk -v s="$s" '$1 ~ (s "$")' all | wc -l)
	[ "$((n + 0))" -ge 1 ] || pc_unresolved "setup: the static symbol $s is not in the object"
done
pc_test
nm -gP x.o >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nm -gP: exit status $rc"
for s in pcfunc pcdata pcbss pcz1 pcy2 pcx3 pcw4; do
	n=$(awk -v s="$s" '$1 ~ (s "$")' o | wc -l)
	[ "$((n + 0))" -eq 1 ] || pc_fail "the global symbol $s is not listed once with -g"
done
for s in pclocal pcstat; do
	n=$(awk -v s="$s" '$1 ~ (s "$")' o | wc -l)
	[ "$((n + 0))" -eq 0 ] || pc_fail "the static symbol $s is listed with -g"
done
# the types a, b, d and t are the local ones
n=$(awk '$2 ~ /^[abdt]$/' o | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a symbol of a local type is listed with -g: $(awk '$2 ~ /^[abdt]$/' o | head -n 1)"
pc_pass
