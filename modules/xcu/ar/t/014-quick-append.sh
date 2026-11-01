# ar -q appends
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.014
# "ar -q a f2" appends f2 after the existing members; "ar -qc" on
# a missing archive creates it.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3; printf 'four\n' >f4
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -q a f2 >out 2>err </dev/null || pc_fail "ar -q: nonzero status"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 f2 ' ] || pc_fail "ar -q did not append f2"
[ "$(ar -p a f2 </dev/null)" = two ] || pc_fail "the appended member has the wrong contents"
[ "$(ar -p a f1 </dev/null)" = one ] || pc_fail "the existing member changed"
# Several operands go to the end, in an order the text leaves open.
ar -q a f4 f3 >out 2>err </dev/null || pc_fail "ar -q a f4 f3: nonzero status"
case $(ar -t a </dev/null | tr '\n' ' ') in 'f1 f2 f4 f3 '|'f1 f2 f3 f4 ') ;; *) pc_fail "ar -q a f4 f3 gives '$(ar -t a </dev/null | tr '\n' ' ')'" ;; esac
[ "$(ar -p a f3 </dev/null)" = three ] && [ "$(ar -p a f4 </dev/null)" = four ] || pc_fail "the appended members have the wrong contents"
# A missing archive is created.
ar -qc b f3 >out 2>err </dev/null || pc_fail "ar -qc creating an archive: nonzero status"
[ "$(ar -t b </dev/null | tr '\n' ' ')" = 'f3 ' ] || pc_fail "ar -qc did not create an archive holding f3"
[ -s err ] && pc_fail "-c did not suppress the diagnostic"
pc_pass
