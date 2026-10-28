# pax -k and -u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.012
# -k leaves an existing file alone; -u extracts a member only when it is
# newer than the existing file.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'archived\n' >f1
touch -t 200101010000 f1 || pc_unresolved "setup: touch failed"
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
printf 'existing\n' >f1
pc_test
pax -r -k <a >o 2>err
[ "$(cat f1)" = existing ] || pc_fail "-k overwrote an existing file"
pax -r -u <a >o 2>err
[ "$(cat f1)" = existing ] || pc_fail "-u replaced a newer file with an older member"
touch -t 199901010000 f1 || pc_unresolved "touch failed"
pax -r -u <a >o 2>err || pc_fail "-u with a newer member: nonzero status"
[ "$(cat f1)" = archived ] || pc_fail "-u did not replace an older file with a newer member"
printf 'existing\n' >f1
pax -r <a >o 2>err || pc_fail "pax -r over an existing file: nonzero status"
[ "$(cat f1)" = archived ] || pc_fail "without -k an existing file was not overwritten"
pc_pass
