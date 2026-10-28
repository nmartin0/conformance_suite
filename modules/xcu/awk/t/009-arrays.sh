# awk arrays are associative with string subscripts, join multiple subscripts with SUBSEP, create elements on reference but not on in, and support delete, for-in and split.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.009
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.009
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'a["x"]=1; a[1]=2; print a["x"], a[1], a["1"], length(a)')" = "1 2 2 2" ] || pc_fail "subscripts or length of an array wrong"
[ "$(e 'a[1,2]=3; for (k in a) { split(k, p, SUBSEP); print p[1], p[2] }; print ((1,2) in a), ((2,1) in a)')" = "1 2${NL}1 0" ] || pc_fail "multi-dimensional subscripts wrong"
[ "$(e 'if (a["q"] == "") n = length(a); print n')" = "1" ] || pc_fail "referencing an element did not create it"
[ "$(e 'if ("q" in a) x = 1; print length(a)')" = "0" ] || pc_fail "the in operator created an element"
[ "$(e 'a[1]; a[2]; delete a[1]; print length(a), (1 in a), (2 in a); delete a; print length(a)')" = "1 0 1${NL}0" ] || pc_fail "delete wrong"
[ "$(e 'for (i=1; i<=5; i++) a[i]=i*i; for (k in a) { n++; s+=a[k] }; print n, s')" = "5 55" ] || pc_fail "for-in did not visit each element once"
[ "$(e 'n=split("a:b:c", p, ":"); print n, p[1], p[3]; n=split("", p); print n, length(p)')" = "3 a c${NL}0 0" ] || pc_fail "split with a separator wrong"
[ "$(e 'n=split("  x  y ", q); print n, q[1], q[2]')" = "2 x y" ] || pc_fail "split with the default FS wrong"
[ "$(e 'n=split("a1b22c", r, /[0-9]+/); print n, r[2]')" = "3 b" ] || pc_fail "split with a regular expression wrong"
[ "$(e 'a[9]="z"; split("p q", a); print length(a), (9 in a)')" = "2 0" ] || pc_fail "split did not delete the old elements"
[ "$(e 'split("10 9", a); print (a[1] > a[2])')" = "1" ] || pc_fail "split elements were not numeric strings"
[ "$(e 'a[0.1+0.2]=1; for (k in a) print k')" = "0.3" ] || pc_fail "a numeric subscript was not converted with CONVFMT"
pc_pass
