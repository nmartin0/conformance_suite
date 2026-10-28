# uudecode decodes the Base64 format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.004
# Data in the begin-base64 format ending in the "====" line decodes to the
# original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin-base64 644 b.out\nQ2F0\n====\n' >in || pc_unresolved "setup: cannot write input"
pc_test
uudecode in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f b.out ] || pc_fail "b.out was not created"
[ "$(cat b.out)" = Cat ] || pc_fail "decoded content is wrong"
pc_pass
