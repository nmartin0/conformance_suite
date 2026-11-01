# ar -ru replaces only if the file is not older
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.007
# With -r -u a file whose modification time is older than that of
# the archived member is not replaced, and one that is at least as
# new (including one exactly as new) is replaced; a file not yet
# in the archive is still added.
# MUTANT    ar.u_ignored
# MUTANT    ar.u_never
# MUTANT    ar.u_strict
# MUTANT    ar.u_reverse
# MUTANT    ar.u_skips_new
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
# A file as new as the archived member is replaced.
printf 'equal\n' >f1
touch -t 200201010000 f1 || pc_unresolved "touch failed"
ar -ru a f1 >out 2>err </dev/null || pc_fail "ar -ru with an equally old file: nonzero status"
[ "$(ar -p a f1 </dev/null)" = equal ] || pc_fail "a file as new as the archived member did not replace it"
printf 'newer\n' >f1
touch -t 200301010000 f1 || pc_unresolved "touch failed"
ar -ru a f1 >out 2>err </dev/null || pc_fail "ar -ru with a newer file: nonzero status"
[ "$(ar -p a f1 </dev/null)" = newer ] || pc_fail "a newer file did not replace the archived member"
# The comparison is with the time of the archived member, which is now 2003.
printf 'older again\n' >f1
touch -t 200201010000 f1 || pc_unresolved "touch failed"
ar -ru a f1 >out 2>err </dev/null || pc_fail "ar -ru with an older file: nonzero status"
[ "$(ar -p a f1 </dev/null)" = newer ] || pc_fail "a file older than the replacing one replaced the member"
# -u only restricts replacement: a file not yet in the archive is added.
printf 'new member\n' >f2
touch -t 200001010000 f2 || pc_unresolved "touch failed"
ar -ru a f2 >out 2>err </dev/null || pc_fail "ar -ru adding an old file: nonzero status"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 ' ] || pc_fail "ar -ru did not add a file that is not in the archive"
[ "$(ar -p a f2 </dev/null)" = 'new member' ] || pc_fail "the added member has the wrong contents"
pc_pass
