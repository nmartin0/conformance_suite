# -r sets the release of the first delta
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.003
# -r3 makes the initial delta 3.1 and the level defaults to 1.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v prs >/dev/null 2>&1 || pc_unresolved "setup: prs is not available"
printf 'x\n' >f || pc_unresolved "setup: cannot write f"
pc_test
admin -r3 -if s.r </dev/null >out 2>err || pc_fail "admin -r3: nonzero status"
got=$(prs -r -d':I:' s.r </dev/null 2>/dev/null)
[ "$got" = 3.1 ] || pc_fail "-r3 gave the initial SID '$got', expected 3.1"
admin -r2.4 -if s.q </dev/null >out 2>err || pc_fail "admin -r2.4: nonzero status"
got=$(prs -r -d':I:' s.q </dev/null 2>/dev/null)
[ "$got" = 2.4 ] || pc_fail "-r2.4 gave the initial SID '$got', expected 2.4"
pc_pass
