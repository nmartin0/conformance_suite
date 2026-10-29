# ls: of exclusive options the last one decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.048
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.048
LC_ALL=POSIX; export LC_ALL
mkdir d d/c || pc_unresolved "cannot create d"
: >d/.h && : >d/a && : >d/b && chmod 755 d/b || pc_unresolved "cannot create the files"
for n in 01 02 03 04 05 06 07 08 09 10 11 12; do : >"w$n" || pc_unresolved "cannot create w$n"; done
mkdir w && mv w[0-9]* w/ || pc_unresolved "cannot fill w"
pc_test
# lines <options...>: the output of ls on d as one string
lines() { ls "$@" d 2>err </dev/null | tr '\n' '|'; }
got=$(lines -A -a)
[ "$got" = '.|..|.h|a|b|c|' ] || pc_fail "ls -A -a: '$got', expected dot and dot-dot too"
got=$(lines -a -A)
[ "$got" = '.h|a|b|c|' ] || pc_fail "ls -a -A: '$got', expected neither dot nor dot-dot"
# the dot file is only for the two checks above: without -A or -a a name that
# begins with a period is written only on an implementation-defined condition
rm d/.h || pc_unresolved "cannot remove d/.h"
got=$(lines -F -p)
[ "$got" = 'a|b|c/|' ] || pc_fail "ls -F -p: '$got', expected a, b and c/ (only -p is in effect)"
got=$(lines -p -F)
[ "$got" = 'a|b*|c/|' ] || pc_fail "ls -p -F: '$got', expected a, b* and c/"
got=$(lines -m -1)
[ "$got" = 'a|b|c|' ] || pc_fail "ls -m -1: '$got', expected one name per line"
got=$(lines -1 -m)
[ "$got" = 'a, b, c|' ] || pc_fail "ls -1 -m: '$got', expected the comma list"
COLUMNS=20; export COLUMNS
ls -C w >c1 2>err </dev/null; ls -x w >x1 2>err </dev/null
ls -C -x w >cx 2>err </dev/null || pc_fail "ls -C -x failed"
ls -x -C w >xc 2>err </dev/null || pc_fail "ls -x -C failed"
cmp -s cx x1 || pc_fail "ls -C -x does not give the output of ls -x"
cmp -s xc c1 || pc_fail "ls -x -C does not give the output of ls -C"
pc_pass
