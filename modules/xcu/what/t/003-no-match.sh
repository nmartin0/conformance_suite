# no match and missing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.what.003
# A file without a match gives the pathname line and status 1; so does a
# nonexistent file.
. "$PC_CORE/pc.sh"
pc_begin XCU.what.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'nothing here\n' >f || pc_unresolved "setup: cannot write f"
pc_test
what f </dev/null >out 2>err
rc=$?
[ "$rc" -eq 1 ] || pc_fail "a file without a match: exit status $rc, expected 1"
[ "$(cat out)" = 'f:' ] || pc_fail "a file without a match wrote '$(cat out)'"
what nosuchfile </dev/null >out 2>err
rc=$?
[ "$rc" -eq 1 ] || pc_fail "a missing file: exit status $rc, expected 1"
pc_pass
