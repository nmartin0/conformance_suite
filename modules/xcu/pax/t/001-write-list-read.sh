# pax -w, list mode and -r round trip
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.001
# "pax -w f1 d" writes an archive to standard output; "pax" with it on
# standard input lists f1, d and d/f2; "pax -r" recreates the files, the
# directory and the contents after the originals are removed.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'file one\n' >f1; mkdir d; printf 'nested\n' >d/f2
norm() { sed 's,/*$,,'; }
pc_test
pax -w f1 d >a.pax 2>err </dev/null || pc_fail "pax -w: nonzero status"
[ -s a.pax ] || pc_fail "pax -w wrote no archive to standard output"
pax <a.pax >lst 2>err || pc_fail "list mode: nonzero status"
got=$(norm <lst | sort | tr '\n' ' ')
[ "$got" = 'd d/f2 f1 ' ] || pc_fail "the archive lists '$got', expected d, d/f2 and f1"
rm -rf f1 d
pax -r <a.pax >o 2>err || pc_fail "read mode: nonzero status"
[ "$(cat f1)" = 'file one' ] || pc_fail "f1 was not restored"
[ -d d ] || pc_fail "directory d was not restored"
[ "$(cat d/f2)" = nested ] || pc_fail "d/f2 was not restored"
pc_pass
