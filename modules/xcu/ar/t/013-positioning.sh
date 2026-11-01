# ar -r with -a, -b and -i positions new files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.013
# "ar -r -a f1 a n" puts new member n after f1; -b puts it before
# the named member; -i behaves as -b.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
for f in f1 f2 f3 n n2; do printf '%s\n' "$f" >$f; done
mk() { rm -f a; pc_mk a f1 f2 f3 || pc_unresolved "setup: cannot build the archive"; }
lst() { ar -t a </dev/null | tr '\n' ' '; }
pc_test
mk
ar -r -a f1 a n >out 2>err </dev/null || pc_fail "ar -r -a: nonzero status"
[ "$(lst)" = 'f1 n f2 f3 ' ] || pc_fail "ar -r -a f1 gives '$(lst)'"
ar -p a n >o 2>/dev/null </dev/null
[ "$(cat o)" = n ] || pc_fail "the positioned member has the wrong contents"
mk
ar -r -a f2 a n >out 2>err </dev/null || pc_fail "ar -r -a f2: nonzero status"
[ "$(lst)" = 'f1 f2 n f3 ' ] || pc_fail "ar -r -a f2 gives '$(lst)'"
mk
ar -r -a f3 a n >out 2>err </dev/null || pc_fail "ar -r -a f3: nonzero status"
[ "$(lst)" = 'f1 f2 f3 n ' ] || pc_fail "ar -r -a f3 gives '$(lst)'"
mk
ar -r -b f1 a n >out 2>err </dev/null || pc_fail "ar -r -b: nonzero status"
[ "$(lst)" = 'n f1 f2 f3 ' ] || pc_fail "ar -r -b f1 gives '$(lst)'"
mk
ar -r -b f2 a n >out 2>err </dev/null || pc_fail "ar -r -b f2: nonzero status"
[ "$(lst)" = 'f1 n f2 f3 ' ] || pc_fail "ar -r -b f2 gives '$(lst)'"
mk
ar -r -b f3 a n >out 2>err </dev/null || pc_fail "ar -r -b f3: nonzero status"
[ "$(lst)" = 'f1 f2 n f3 ' ] || pc_fail "ar -r -b f3 gives '$(lst)'"
mk
ar -r -i f3 a n >out 2>err </dev/null || pc_fail "ar -r -i: nonzero status"
[ "$(lst)" = 'f1 f2 n f3 ' ] || pc_fail "ar -r -i f3 gives '$(lst)'"
mk
ar -r -i f1 a n >out 2>err </dev/null || pc_fail "ar -r -i f1: nonzero status"
[ "$(lst)" = 'n f1 f2 f3 ' ] || pc_fail "ar -r -i f1 gives '$(lst)'"
# Two new files: both go after the named member, in an order the text leaves open.
mk
ar -r -a f1 a n n2 >out 2>err </dev/null || pc_fail "ar -r -a f1 a n n2: nonzero status"
case $(lst) in 'f1 n n2 f2 f3 '|'f1 n2 n f2 f3 ') ;; *) pc_fail "ar -r -a f1 a n n2 gives '$(lst)'" ;; esac
mk
ar -r -b f3 a n n2 >out 2>err </dev/null || pc_fail "ar -r -b f3 a n n2: nonzero status"
case $(lst) in 'f1 f2 n n2 f3 '|'f1 f2 n2 n f3 ') ;; *) pc_fail "ar -r -b f3 a n n2 gives '$(lst)'" ;; esac
pc_pass
