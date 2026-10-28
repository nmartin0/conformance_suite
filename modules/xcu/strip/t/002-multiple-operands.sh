# strip with several operands
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strip.002
# "strip p1 p2" exits 0 and both programs still run correctly.
. "$PC_CORE/pc.sh"
pc_begin XCU.strip.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >p1.c <<'EOC'
#include <stdio.h>
int main(void) { puts("one"); return 0; }
EOC
cat >p2.c <<'EOC'
#include <stdio.h>
int main(void) { puts("two"); return 0; }
EOC
c17 -o p1 p1.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 failed"
c17 -o p2 p2.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 failed"
pc_test
strip p1 p2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "strip p1 p2: exit status $rc"
[ "$(./p1 </dev/null)" = one ] || pc_fail "p1 no longer works after strip"
[ "$(./p2 </dev/null)" = two ] || pc_fail "p2 no longer works after strip"
pc_pass
