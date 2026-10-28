# ar -r creates an archive; -c suppresses the diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.001
# "ar -r a f1" creating a new archive exits 0, writes a diagnostic to
# standard error and the archive then lists f1; with -c ("ar -rc") nothing
# is written to standard error.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1 || pc_unresolved "setup: cannot write f1"
pc_test
ar -r a1 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -r creating an archive: exit status $rc"
[ -s err ] || pc_fail "no diagnostic about creating the archive"
[ "$(ar -t a1 </dev/null)" = f1 ] || pc_fail "the new archive does not list f1"
ar -rc a2 f1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -rc: exit status $rc"
[ -s err ] && pc_fail "-c did not suppress the diagnostic"
[ "$(ar -t a2 </dev/null)" = f1 ] || pc_fail "archive made with -c does not list f1"
pc_pass
