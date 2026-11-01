# nm -A prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.004
# With -A -P each line starts with the file operand (or its full pathname)
# followed by a colon and a blank; for a library the member that holds the
# symbol follows the library in square brackets. No identifying lines are
# written with -A.
# MUTANT    nm.A_ignored
# MUTANT    nm.A_no_colon
# MUTANT    nm.A_no_blank
# MUTANT    nm.A_basename
# MUTANT    nm.A_first_file
# MUTANT    nm.A_headers_too
# MUTANT    nm.A_lib_no_member
# MUTANT    nm.A_lib_paren
# MUTANT    nm.A_lib_first_member
# MUTANT    nm.A_first_line_only
# MUTANT    nm.A_undef_bare
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.A_tab
# VARIANT   nm.A_absolute
# VARIANT   nm.prefixed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.004
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
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
c17 -c y.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] && [ -f y.o ] || pc_unresolved "setup: c17 -c produced no object"
mkdir sub && cp x.o sub/x.o || pc_unresolved "setup: cannot copy the object"
ar -rc libx.a x.o y.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
# pc_pfx FILE SYMBOL PREFIX: the line of SYMBOL starts with PREFIX (or a full pathname ending in it)
pc_pfx() {
	n=$(awk -v s="$2" -v p="$3" '$2 ~ (s "$") && ($1 == p || (length($1) > length(p) + 1 && substr($1, length($1) - length(p)) == "/" p)) { c++ } END { print c + 0 }' "$1")
	[ "$n" -eq 1 ] || pc_fail "$1: the line of $2 does not start with $3 and a blank"
}
# pc_allpfx FILE PREFIX: every line has a name and a type after a prefix like that
pc_allpfx() {
	n=$(awk -v p="$2" 'NF < 3 || !($1 == p || (length($1) > length(p) + 1 && substr($1, length($1) - length(p)) == "/" p)) { c++ } END { print c + 0 }' "$1")
	[ "$n" -eq 0 ] || pc_fail "$1: a line does not start with $2 and carry a name and a type"
}
nm -AP sub/x.o >o1 2>err </dev/null || pc_fail "nm -AP sub/x.o: nonzero status"
[ -s o1 ] || pc_fail "nm -AP wrote nothing"
pc_allpfx o1 'sub/x.o:'
pc_pfx o1 pcfunc 'sub/x.o:'
pc_pfx o1 pcundef 'sub/x.o:'
nm -AP x.o y.o >o2 2>err </dev/null || pc_fail "nm -AP x.o y.o: nonzero status"
pc_pfx o2 pcfunc 'x.o:'
pc_pfx o2 pcdata 'x.o:'
pc_pfx o2 pcother 'y.o:'
n=$(awk 'NF < 3' o2 | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a line without a prefix and a symbol (an identifying line) is written with -A"
nm -AP libx.a >o3 2>err </dev/null || pc_fail "nm -AP libx.a: nonzero status"
[ -s o3 ] || pc_fail "nm -AP of a library wrote nothing"
pc_pfx o3 pcfunc 'libx.a[x.o]:'
pc_pfx o3 pcdata 'libx.a[x.o]:'
pc_pfx o3 pcundef 'libx.a[x.o]:'
pc_pfx o3 pcother 'libx.a[y.o]:'
n=$(awk 'NF < 3' o3 | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a line without a prefix and a symbol is written for a library with -A"
pc_pass
