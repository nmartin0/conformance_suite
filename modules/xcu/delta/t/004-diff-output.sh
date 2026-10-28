# -p
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.delta.004
# -p writes the differences in diff format to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.delta.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
get -e -s s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
printf 'four\n' >>f || pc_unresolved "setup: cannot edit f"
pc_test
delta -p -y'add four' s.f </dev/null >out 2>err || pc_fail "delta -p: nonzero status"
grep -E '^[>+] ?four$' out >/dev/null || pc_fail "no diff line for the added text: $(cat out)"
pc_pass
