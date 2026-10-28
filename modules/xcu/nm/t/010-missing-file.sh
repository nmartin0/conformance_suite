# nm of a nonexistent file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.010
# "nm" on a file that does not exist exits with a status greater than zero.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
:
pc_test
nm nonexistent.o >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "nm of a nonexistent file gave status 0"
pc_pass
