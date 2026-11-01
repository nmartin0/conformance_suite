# ar -r with a position option does not move replaced members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.020
# With "ar -r -a posname" or "-b" or "-i", a file that replaces an
# existing member keeps that member's place and takes the new
# contents; only files that are new are positioned.
# MUTANT    ar.ra_moves_replaced
# MUTANT    ar.rb_moves_replaced
# MUTANT    ar.ri_moves_replaced
# MUTANT    ar.rpos_replaced_dup
# MUTANT    ar.rpos_replaced_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
for f in f1 f2 f3 n; do printf '%s\n' "$f" >$f; done
mk() { rm -f a; pc_mk a f1 f2 f3 || pc_unresolved "setup: cannot build the archive"; }
lst() { ar -t a </dev/null | tr '\n' ' '; }
pc_test
mk
printf 'f1 again\n' >f1
ar -r -a f3 a f1 n >out 2>err </dev/null || pc_fail "ar -r -a f3 a f1 n: nonzero status"
[ "$(lst)" = 'f1 f2 f3 n ' ] || pc_fail "ar -r -a f3 a f1 n gives '$(lst)', the replaced f1 must stay first"
ar -p a f1 >o 2>/dev/null </dev/null
[ "$(cat o)" = 'f1 again' ] || pc_fail "the replaced member f1 has the old contents"
mk
printf 'f3 again\n' >f3
ar -r -b f1 a f3 >out 2>err </dev/null || pc_fail "ar -r -b f1 a f3: nonzero status"
[ "$(lst)" = 'f1 f2 f3 ' ] || pc_fail "ar -r -b f1 a f3 gives '$(lst)', the replaced f3 must stay last"
ar -p a f3 >o 2>/dev/null </dev/null
[ "$(cat o)" = 'f3 again' ] || pc_fail "the replaced member f3 has the old contents"
mk
printf 'f2 again\n' >f2
ar -r -i f1 a n f2 >out 2>err </dev/null || pc_fail "ar -r -i f1 a n f2: nonzero status"
[ "$(lst)" = 'n f1 f2 f3 ' ] || pc_fail "ar -r -i f1 a n f2 gives '$(lst)', the replaced f2 must stay in its place"
pc_pass
