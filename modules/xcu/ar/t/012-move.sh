# ar -m moves members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.012
# "ar -m a f1" moves f1 to the end; "ar -m -a f3 a f1" places it
# after f3, and "ar -m -b f2 a f3" places f3 before f2, other
# members keeping their order.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
for f in f1 f2 f3 f4; do printf '%s\n' "$f" >$f; done
mk() { rm -f a; pc_mk a f1 f2 f3 f4 || pc_unresolved "setup: cannot build the archive"; }
lst() { ar -t a </dev/null | tr '\n' ' '; }
pc_test
mk
ar -m a f1 >out 2>err </dev/null || pc_fail "ar -m: nonzero status"
[ "$(lst)" = 'f2 f3 f4 f1 ' ] || pc_fail "ar -m a f1 gives '$(lst)'"
ar -p a f1 >o 2>/dev/null </dev/null
[ "$(cat o)" = f1 ] || pc_fail "ar -m changed the contents of the moved member"
mk
ar -m a f2 >out 2>err </dev/null || pc_fail "ar -m a f2: nonzero status"
[ "$(lst)" = 'f1 f3 f4 f2 ' ] || pc_fail "ar -m a f2 gives '$(lst)'"
# Several operands: both go to the end, the order between them is open.
mk
ar -m a f1 f3 >out 2>err </dev/null || pc_fail "ar -m a f1 f3: nonzero status"
case $(lst) in 'f2 f4 f1 f3 '|'f2 f4 f3 f1 ') ;; *) pc_fail "ar -m a f1 f3 gives '$(lst)'" ;; esac
mk
ar -m -a f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -a: nonzero status"
[ "$(lst)" = 'f1 f3 f2 f4 ' ] || pc_fail "ar -m -a f1 a f3 gives '$(lst)'"
mk
ar -m -a f3 a f1 >out 2>err </dev/null || pc_fail "ar -m -a f3 a f1: nonzero status"
[ "$(lst)" = 'f2 f3 f1 f4 ' ] || pc_fail "ar -m -a f3 a f1 gives '$(lst)'"
mk
ar -m -a f4 a f1 >out 2>err </dev/null || pc_fail "ar -m -a f4 a f1: nonzero status"
[ "$(lst)" = 'f2 f3 f4 f1 ' ] || pc_fail "ar -m -a f4 a f1 gives '$(lst)'"
mk
ar -m -b f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -b: nonzero status"
[ "$(lst)" = 'f3 f1 f2 f4 ' ] || pc_fail "ar -m -b f1 a f3 gives '$(lst)'"
mk
ar -m -b f4 a f1 >out 2>err </dev/null || pc_fail "ar -m -b f4 a f1: nonzero status"
[ "$(lst)" = 'f2 f3 f1 f4 ' ] || pc_fail "ar -m -b f4 a f1 gives '$(lst)'"
mk
ar -m -b f2 a f4 >out 2>err </dev/null || pc_fail "ar -m -b f2 a f4: nonzero status"
[ "$(lst)" = 'f1 f4 f2 f3 ' ] || pc_fail "ar -m -b f2 a f4 gives '$(lst)'"
mk
ar -m -i f1 a f3 >out 2>err </dev/null || pc_fail "ar -m -i: nonzero status"
[ "$(lst)" = 'f3 f1 f2 f4 ' ] || pc_fail "ar -m -i f1 a f3 gives '$(lst)'"
mk
ar -m -i f4 a f1 >out 2>err </dev/null || pc_fail "ar -m -i f4 a f1: nonzero status"
[ "$(lst)" = 'f2 f3 f1 f4 ' ] || pc_fail "ar -m -i f4 a f1 gives '$(lst)'"
# Several operands with a position: both are placed there.
mk
ar -m -a f2 a f4 f1 >out 2>err </dev/null || pc_fail "ar -m -a f2 a f4 f1: nonzero status"
case $(lst) in 'f2 f4 f1 f3 '|'f2 f1 f4 f3 ') ;; *) pc_fail "ar -m -a f2 a f4 f1 gives '$(lst)'" ;; esac
for f in f1 f2 f3 f4; do
	[ "$(ar -p a $f </dev/null 2>/dev/null)" = $f ] || pc_fail "member $f lost its contents"
done
pc_pass
