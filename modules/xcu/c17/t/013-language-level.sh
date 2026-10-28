# c17 accepts source of the ISO C standard
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.013
# A source file that requires __STDC__ to be 1 and __STDC_VERSION__ to be at
# least 201710L compiles without error.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >s.c <<'EOC'
#if !defined(__STDC__) || __STDC__ != 1
#error not a standard C implementation
#endif
#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 201710L
#error language level is below C17
#endif
int main(void) { return 0; }
EOC
pc_test
c17 -o p s.c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "the language-level check failed to compile: exit status $rc"
pc_pass
