# unreadable pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.msgfmt.007
# A pathname operand that cannot be read gives a non-zero status.
. "$PC_CORE/pc.sh"
pc_begin XCU.msgfmt.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
msgfmt -o out.mo nosuch.po </dev/null >o 2>e
[ "$?" -gt 0 ] || pc_fail "an unreadable pathname gave status 0"
pc_pass
