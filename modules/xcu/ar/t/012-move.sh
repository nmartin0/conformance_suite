# ar -m moves members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.012
# "ar -m a f1" moves f1 to the end; "ar -m -a f3 a f1" places it after f3,
# and "ar -m -b f2 a f3" places f3 before f2, other members keeping their
# order.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for f in f1 f2 f3 f4; do printf '%s\n' "$f" >$f; done
mk() { rm -f a; ar -rc a f1 f2 f3 f4 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"; }
lst() { ar -t a </dev/null | tr '\n' ' '; }
pc_test
mk
ar -m a f1 >out 2>err </dev/null || pc_fail "ar -m: nonzero status"
[ "$(lst)" = 'f2 f3 f4 f1 ' ] || pc_fail "ar -m a f1 gives '$(lst)'"
mk
ar -m -a f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -a: nonzero status"
[ "$(lst)" = 'f1 f3 f2 f4 ' ] || pc_fail "ar -m -a f1 a f3 gives '$(lst)'"
mk
ar -m -b f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -b: nonzero status"
[ "$(lst)" = 'f3 f1 f2 f4 ' ] || pc_fail "ar -m -b f1 a f3 gives '$(lst)'"
mk
ar -m -i f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -i: nonzero status"
[ "$(lst)" = 'f3 f1 f2 f4 ' ] || pc_fail "ar -m -i f1 a f3 gives '$(lst)'"
pc_pass
