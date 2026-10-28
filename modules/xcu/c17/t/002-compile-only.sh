# c17 -c makes object files and no executable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.002
# "c17 -c a.c b.c" creates a.o and b.o and no a.out; "c17 -c -o x.o a.c"
# creates x.o; the objects link to a working program.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'int fa(void) { return 1; }\n' >a.c
printf 'int fb(void) { return 2; }\n' >b.c
printf 'int fa(void);\nint fb(void);\nint main(void) { return fa() * 10 + fb(); }\n' >m.c
pc_test
c17 -c a.c b.c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "c17 -c a.c b.c: exit status $rc"
[ -f a.o ] && [ -f b.o ] || pc_fail "a.o and b.o were not both created"
[ -e a.out ] && pc_fail "-c created an executable"
c17 -c -o x.o a.c >out 2>err </dev/null || pc_fail "c17 -c -o x.o: nonzero status"
[ -f x.o ] || pc_fail "-o x.o was not honoured with -c"
c17 -c m.c >out 2>err </dev/null || pc_fail "c17 -c m.c: nonzero status"
c17 -o prog m.o a.o b.o >out 2>err </dev/null || pc_fail "linking the objects: nonzero status"
./prog </dev/null
rc=$?
[ "$rc" -eq 12 ] || pc_fail "the linked program exits with $rc, expected 12"
pc_pass
