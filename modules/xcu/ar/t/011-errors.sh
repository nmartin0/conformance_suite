# ar failure exit statuses
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.011
# "ar -t" on an archive that does not exist, and "ar -r" naming a file that
# does not exist, each exit with a status greater than zero.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
pc_test
ar -t nonexistent.a >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ar -t of a nonexistent archive gave status 0"
ar -rc b nonexistent.file >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ar -r of a nonexistent file gave status 0"
pc_pass
