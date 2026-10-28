# file names from standard input and errors
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.008
# A single - operand reads SCCS file names from standard input; an
# unreadable -i file gives a non-zero status and creates nothing.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v get >/dev/null 2>&1 || pc_unresolved "setup: get is not available"
pc_test
printf 's.a\ns.b\n' | admin -n - >out 2>err || pc_fail "admin -n - : nonzero status"
[ -f s.a ] && [ -f s.b ] || pc_fail "names read from standard input did not create s.a and s.b"
admin -inosuchinput s.x </dev/null >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "an unreadable -i file gave status 0"
[ -e s.x ] && pc_fail "s.x was created although admin failed"
pc_pass
