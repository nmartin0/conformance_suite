# pax -p o and -p e restore ownership; large ids go in extended records
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.039
# A file owned by ids 61234 and 61235 with mode 4755: extracted with -p o it
# keeps those ids, extracted by default it belongs to the invoking user,
# with -p e it keeps the ids and the mode 4755; in the pax format ids above
# 2097151 are written as uid and gid records.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.039
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'umask 022; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
umask 022
me=$(id -u) || pc_unresolved "setup: id -u failed"
printf 'x\n' >f1 && printf 'y\n' >f2 || pc_unresolved "setup: cannot write files"
chown 61234:61235 f1 && chmod 4755 f1 || pc_unresolved "setup: chown or chmod failed"
chown 3000000:3000001 f2 || pc_unresolved "setup: chown to large ids failed"
owner() { ls -ln "$1" | awk '{ print $3 ":" $4 }'; }
[ "$(owner f1)" = 61234:61235 ] || pc_unresolved "setup: f1 is owned by $(owner f1)"
[ "$(ls -l f1 | cut -c1-10)" = '-rwsr-xr-x' ] || pc_unresolved "setup: f1 has mode $(ls -l f1 | cut -c1-10)"
pax -w f1 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
pax -w -x pax f2 >b 2>/dev/null </dev/null || pc_unresolved "setup: pax -w -x pax failed"
rm -f f1
pc_test
pax -r -p o <a >o 2>err
[ "$(owner f1)" = 61234:61235 ] || pc_fail "-p o: the extracted file is owned by '$(owner f1)', expected 61234:61235"
rm -f f1
pax -r <a >o 2>err
set -- $(owner f1 | tr : ' ')
[ "$1" = "$me" ] || pc_fail "by default the extracted file belongs to user '$1', expected the invoking user $me"
rm -f f1
pax -r -p e <a >o 2>err
[ "$(owner f1)" = 61234:61235 ] || pc_fail "-p e: the extracted file is owned by '$(owner f1)', expected 61234:61235"
[ "$(ls -l f1 | cut -c1-10)" = '-rwsr-xr-x' ] || pc_fail "-p e: the extracted file has mode '$(ls -l f1 | cut -c1-10)', expected -rwsr-xr-x"
tr -d '\000' <b | grep 'uid=3000000' >/dev/null || pc_fail "no uid record for the owner id 3000000"
tr -d '\000' <b | grep 'gid=3000001' >/dev/null || pc_fail "no gid record for the group id 3000001"
rm -f f2
pax -r -p o <b >o 2>err
[ "$(owner f2)" = 3000000:3000001 ] || pc_fail "-p o: the file with large ids is owned by '$(owner f2)', expected 3000000:3000001"
pc_pass
