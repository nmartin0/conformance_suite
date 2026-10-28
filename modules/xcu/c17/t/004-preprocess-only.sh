# c17 -E
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.004
# "c17 -E e.c" writes the preprocessed text to standard output with macros
# expanded and the included header's declaration present, and creates no
# object or executable file.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub || pc_unresolved "setup: mkdir failed"
printf 'int pchdr;\n' >h.h
printf '#include "h.h"\n#define TWICE(x) ((x)+(x))\nint v = TWICE(21);\n' >e.c
pc_test
c17 -E e.c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "c17 -E: exit status $rc"
tr -d ' \t' <out >out.ns
grep -F '((21)+(21))' out.ns >/dev/null || pc_fail "the macro was not expanded"
grep -F 'intpchdr' out.ns >/dev/null || pc_fail "the included declaration is missing"
[ -e e.o ] && pc_fail "-E created an object file"
[ -e a.out ] && pc_fail "-E created an executable"
pc_pass
