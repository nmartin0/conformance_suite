# ar -r with -a, -b and -i positions new files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.013
# "ar -r -a f1 a n" puts new member n after f1; -b puts it before the named
# member; -i behaves as -b.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
for f in f1 f2 f3 n; do printf '%s\n' "$f" >$f; done
mk() { rm -f a; ar -rc a f1 f2 f3 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"; }
lst() { ar -t a </dev/null | tr '\n' ' '; }
pc_test
mk
ar -r -a f1 a n >out 2>err </dev/null || pc_fail "ar -r -a: nonzero status"
[ "$(lst)" = 'f1 n f2 f3 ' ] || pc_fail "ar -r -a f1 gives '$(lst)'"
mk
ar -r -b f1 a n >out 2>err </dev/null || pc_fail "ar -r -b: nonzero status"
[ "$(lst)" = 'n f1 f2 f3 ' ] || pc_fail "ar -r -b f1 gives '$(lst)'"
mk
ar -r -i f3 a n >out 2>err </dev/null || pc_fail "ar -r -i: nonzero status"
[ "$(lst)" = 'f1 f2 n f3 ' ] || pc_fail "ar -r -i f3 gives '$(lst)'"
pc_pass
