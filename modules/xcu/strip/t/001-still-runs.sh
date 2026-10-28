# a stripped executable still runs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strip.001
# After "strip prog" (status 0) the program still runs and gives the same
# output and exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.strip.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >p1.c <<'EOC'
#include <stdio.h>
int main(void) { puts("ok"); return 7; }
EOC
c17 -o p1 p1.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 failed"
./p1 >p1.out </dev/null
[ "$?" -eq 7 ] && [ "$(cat p1.out)" = ok ] || pc_unresolved "setup: the program does not run as expected"
pc_test
strip p1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "strip: exit status $rc"
./p1 >p1.out2 </dev/null
rc=$?
[ "$rc" -eq 7 ] || pc_fail "the stripped program exits with $rc, expected 7"
[ "$(cat p1.out2)" = ok ] || pc_fail "the stripped program output differs"
pc_pass
