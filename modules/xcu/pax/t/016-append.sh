# pax -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.016
# "pax -w -a -f A f2" adds f2 to an existing archive A holding f1, and
# listing shows f1 then f2; appending in a different format to an existing
# archive exits with a nonzero status.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2
pc_test
pax -w -f A f1 >o 2>err </dev/null || pc_fail "pax -w -f A: nonzero status"
pax -w -a -f A f2 >o 2>err </dev/null || pc_fail "pax -w -a: nonzero status"
pax -f A >lst 2>err </dev/null || pc_fail "listing the appended archive: nonzero status"
[ "$(cat lst | tr '\n' ' ')" = 'f1 f2 ' ] || pc_fail "the appended archive lists '$(tr '\n' ' ' <lst)'"
pax -w -x ustar -f B f1 >o 2>err </dev/null || pc_fail "pax -w -x ustar -f B: nonzero status"
pax -w -a -x cpio -f B f2 >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "appending in another format gave status 0"
pc_pass
