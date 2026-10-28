# c17 compiles, links and names the executable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.001
# "c17 p.c" writes the executable a.out, "c17 -o prog p.c" writes prog, and
# running either gives the program's output and its exit status; the
# executable has permission bits rwx for all, with the umask cleared from
# them (0755 under umask 022, 0700 under umask 077).
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >p.c <<'EOC'
#include <stdio.h>
int main(void) { puts("hi"); return 5; }
EOC
pc_test
c17 p.c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "c17 p.c: exit status $rc"
[ -f a.out ] || pc_fail "a.out was not created"
./a.out >o1 </dev/null
rc=$?
[ "$rc" -eq 5 ] || pc_fail "a.out exits with $rc, expected 5"
[ "$(cat o1)" = hi ] || pc_fail "a.out printed '$(cat o1)'"
c17 -o prog p.c >out 2>err </dev/null || pc_fail "c17 -o prog: nonzero status"
[ -f prog ] || pc_fail "prog was not created"
./prog >o2 </dev/null
rc=$?
[ "$rc" -eq 5 ] || pc_fail "prog exits with $rc, expected 5"
umask 022
rm -f prog
c17 -o prog p.c >out 2>err </dev/null || pc_fail "c17 under umask 022: nonzero status"
perms=$(ls -l prog | cut -c1-10)
[ "$perms" = '-rwxr-xr-x' ] || pc_fail "mode under umask 022 is $perms"
umask 077
rm -f prog
c17 -o prog p.c >out 2>err </dev/null || pc_fail "c17 under umask 077: nonzero status"
umask 022
perms=$(ls -l prog | cut -c1-10)
[ "$perms" = '-rwx------' ] || pc_fail "mode under umask 077 is $perms"
pc_pass
