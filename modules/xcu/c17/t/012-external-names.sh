# external names significant to 31 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.012
# Two functions whose 31-byte external names differ only in the last byte,
# defined in separate files, link into one program in which both are
# callable and distinct.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'int pc_long_external_symbol_name_a1(void) { return 1; }\n' >e1.c
printf 'int pc_long_external_symbol_name_a2(void) { return 2; }\n' >e2.c
printf 'int pc_long_external_symbol_name_a1(void);\nint pc_long_external_symbol_name_a2(void);\nint main(void) { return pc_long_external_symbol_name_a1() * 10 + pc_long_external_symbol_name_a2(); }\n' >em.c
pc_test
c17 -o p em.c e1.c e2.c >out 2>err </dev/null || pc_fail "linking two 31-byte names: nonzero status"
./p </dev/null
rc=$?
[ "$rc" -eq 12 ] || pc_fail "the program exits with $rc, expected 12"
pc_pass
