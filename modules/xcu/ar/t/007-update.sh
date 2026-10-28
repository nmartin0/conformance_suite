# ar -ru replaces only if the file is not older
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.007
# With -r -u a file whose modification time is older than that of the
# archived member is not replaced, and one that is at least as new is
# replaced.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
touch -t 200201010000 f1 || pc_unresolved "setup: touch failed"
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
printf 'older\n' >f1
touch -t 200101010000 f1 || pc_unresolved "setup: touch failed"
pc_test
ar -ru a f1 >out 2>err </dev/null || pc_fail "ar -ru with an older file: nonzero status"
[ "$(ar -p a f1 </dev/null)" = one ] || pc_fail "an older file replaced the archived member"
printf 'newer\n' >f1
touch -t 200301010000 f1 || pc_unresolved "touch failed"
ar -ru a f1 >out 2>err </dev/null || pc_fail "ar -ru with a newer file: nonzero status"
[ "$(ar -p a f1 </dev/null)" = newer ] || pc_fail "a newer file did not replace the archived member"
pc_pass
