# ar -x -C does not replace existing files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.015
# "ar -x -C a f1" leaves an existing f1 with its old contents; without -C it
# is replaced.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
printf 'changed\n' >f1
pc_test
ar -x -C a f1 >out 2>err </dev/null
[ "$(cat f1)" = changed ] || pc_fail "-C let the extracted file replace an existing one"
ar -x a f1 >out 2>err </dev/null || pc_fail "ar -x without -C: nonzero status"
[ "$(cat f1)" = one ] || pc_fail "without -C the existing file was not replaced"
pc_pass
