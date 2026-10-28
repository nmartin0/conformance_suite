# pax -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.002
# -f names the archive in write, list and read mode instead of standard
# output or input.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'file one\n' >f1; mkdir d; printf 'nested\n' >d/f2
pc_test
pax -w -f a.pax f1 d >o 2>err </dev/null || pc_fail "pax -w -f: nonzero status"
[ -s o ] && pc_fail "pax -w -f wrote to standard output"
[ -s a.pax ] || pc_fail "pax -w -f wrote no archive file"
pax -f a.pax >lst 2>err </dev/null || pc_fail "pax -f (list): nonzero status"
got=$(sed 's,/*$,,' lst | sort | tr '\n' ' ')
[ "$got" = 'd d/f2 f1 ' ] || pc_fail "pax -f lists '$got'"
rm -rf f1 d
pax -r -f a.pax >o 2>err </dev/null || pc_fail "pax -r -f: nonzero status"
[ "$(cat d/f2)" = nested ] && [ "$(cat f1)" = 'file one' ] || pc_fail "pax -r -f did not restore the files"
pc_pass
