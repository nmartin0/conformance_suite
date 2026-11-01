# nm identifying lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.005
# Without -A, "nm -P x.o y.o z.o" prints "x.o:", "y.o:" and "z.o:" lines
# before each file's symbols, and "nm -P libx.a" for a library prints
# "libx.a[x.o]:" before the symbols of that member (also for a library of one
# member); a single non-library file has no such line.
# MUTANT    nm.h_none
# MUTANT    nm.h_first_only
# MUTANT    nm.h_not_last
# MUTANT    nm.h_no_colon
# MUTANT    nm.h_lib_no_member
# MUTANT    nm.h_lib_paren
# MUTANT    nm.h_first_name
# MUTANT    nm.h_after
# MUTANT    nm.h_single
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >x.c <<'EOC'
int pcdata[4] = {1, 2, 3, 4};
extern int pcundef(void);
int pcfunc(void) { return pcundef() + pcdata[0]; }
EOC
printf 'int pcother(void) { return 2; }\n' >y.c
printf 'int pcthird(void) { return 3; }\n' >z.c
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
c17 -c y.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
c17 -c z.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] && [ -f y.o ] && [ -f z.o ] || pc_unresolved "setup: c17 -c produced no object"
ar -rc libx.a x.o y.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
ar -rc liby.a y.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
# pc_under FILE SYMBOL HEADER: the line of SYMBOL comes after HEADER (an identifying line) and before the next one
pc_under() {
	h=$(awk -v s="$2" 'NF == 1 && $1 ~ /:$/ { h = $1; next } NF >= 2 && $1 ~ (s "$") { print h }' "$1")
	[ "$h" = "$3" ] || pc_fail "$1: the symbol $2 is under '$h', expected '$3'"
}
nm -P x.o y.o z.o >o 2>err </dev/null || pc_fail "nm -P x.o y.o z.o: nonzero status"
for f in x.o y.o z.o; do
	grep -x -F "$f:" o >/dev/null || pc_fail "no $f: line"
done
pc_under o pcfunc 'x.o:'
pc_under o pcdata 'x.o:'
pc_under o pcother 'y.o:'
pc_under o pcthird 'z.o:'
nm -P x.o >o2 2>err </dev/null || pc_fail "nm -P x.o: nonzero status"
[ -s o2 ] || pc_fail "nm -P x.o wrote nothing"
n=$(awk 'NF < 2' o2 | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a single object file got an identifying (or empty) line"
nm -P libx.a >o3 2>err </dev/null || pc_fail "nm -P libx.a: nonzero status"
grep -x -F 'libx.a[x.o]:' o3 >/dev/null || pc_fail "no libx.a[x.o]: line for the library"
grep -x -F 'libx.a[y.o]:' o3 >/dev/null || pc_fail "no libx.a[y.o]: line for the library"
pc_under o3 pcfunc 'libx.a[x.o]:'
pc_under o3 pcother 'libx.a[y.o]:'
nm -P liby.a >o4 2>err </dev/null || pc_fail "nm -P liby.a: nonzero status"
grep -x -F 'liby.a[y.o]:' o4 >/dev/null || pc_fail "no liby.a[y.o]: line for a library of one member"
pc_under o4 pcother 'liby.a[y.o]:'
pc_pass
