# pax: access times are restored by default and not with -p a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.027
# A file with a preset access time archived in the pax format with -o times
# gets that access time back on extraction by default, and does not get it
# with -p a.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.027
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >f1 || pc_unresolved "setup: cannot write f1"
touch -a -d 2001-09-09T01:46:40Z f1 || pc_unresolved "setup: touch -a -d failed"
ls -lu f1 | grep 2001 >/dev/null || pc_unresolved "setup: the access time could not be set"
pax -w -t -x pax -o times f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
tr -d '\000' <a | grep -E '^[0-9]+ atime=1000000000(\.0*)?$' >/dev/null || pc_unresolved "setup: the archive does not hold the preset access time (the read changed it)"
rm -f f1
pc_test
pax -r <a >o 2>err
[ -f f1 ] || pc_fail "pax -r did not extract f1"
ls -lu f1 | grep 2001 >/dev/null || pc_fail "by default the archived access time was not restored: $(ls -lu f1)"
rm -f f1
pax -r -p a <a >o 2>err
[ -f f1 ] || pc_fail "pax -r -p a did not extract f1"
ls -lu f1 | grep 2001 >/dev/null && pc_fail "-p a still restored the archived access time: $(ls -lu f1)"
pc_pass
