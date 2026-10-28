# a delta being edited is kept
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rmdel.003
# rmdel of a SID that is listed in the p-file gives a non-zero status and
# leaves the file unchanged.
. "$PC_CORE/pc.sh"
pc_begin XCU.rmdel.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
pc_test
rmdel -r1.1 s.f </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_unresolved "removing a delta that is being edited succeeded: the application is to ensure this does not happen, so the result is not specified"
get -s -p -r1.1 s.f </dev/null >out 2>err || pc_fail "get -r1.1: nonzero status"
printf '%b' 'one\ntwo\nthree\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "delta 1.1 after the refused rmdel: $(od -c out | head -3)"
pc_pass
