# missing file and missing SID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdel.004
# A nonexistent file or a nonexistent SID gives a non-zero status.
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdel.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
rmdel -r1.1 nosuch.f </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "a nonexistent file gave status 0"
rmdel -r1.9 s.f </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "a nonexistent SID gave status 0"
get -s -p  s.f </dev/null >out 2>err || pc_fail "get : nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "the text after the refused requests: $(od -c out | head -3)"
pc_pass
