# -n alone and -i from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.admin.002
# -n creates a file with no text and -i without a name reads the text from
# standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.admin.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v get >/dev/null 2>&1 || pc_unresolved "setup: get is not available"
printf 'a\nb\n' >in || pc_unresolved "setup: cannot write in"
pc_test
admin -n s.e </dev/null >out 2>err || pc_fail "admin -n: nonzero status"
[ -f s.e ] || pc_fail "admin -n did not create s.e"
get -s -p s.e </dev/null >got 2>err
[ -s got ] && pc_fail "the file made with -n alone has text"
admin -i s.g <in >out 2>err || pc_fail "admin -i (stdin): nonzero status"
[ -f s.g ] || pc_fail "admin -i did not create s.g"
get -s -p s.g </dev/null >got 2>err || pc_fail "get cannot read s.g"
cmp got in >/dev/null 2>&1 || pc_fail "the text taken from standard input differs"
pc_pass
