# delta without a get -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.005
# delta on a file with no pending edit gives a non-zero status and changes
# nothing.
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
delta -y'nothing' s.f </dev/null >out 2>err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "delta without a pending edit gave status 0"
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the text after the failed delta: $(od -c out | head -3)"
get -s -p -r1.2 s.f </dev/null >out 2>err && pc_fail "a delta 1.2 exists after the failed delta"
pc_pass
