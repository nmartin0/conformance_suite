# talk with standard input that is not a terminal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.talk.001
. "$PC_CORE/pc.sh"
pc_begin XCU.talk.001
LC_ALL=C; export LC_ALL
d=pctalk.$$
mkdir "$d" || pc_fail "cannot make $d"
trap 'rm -rf "$d"' EXIT
pc_test
talk pc_no_such_login_zz </dev/null >"$d/out" 2>"$d/err"
rc=$?
[ "$rc" -gt 0 ] || pc_fail "talk with standard input that is not a terminal exited with status 0"
[ -s "$d/out" ] || [ -s "$d/err" ] || pc_fail "talk wrote no diagnostic message"
pc_pass
