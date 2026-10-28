# User-defined functions can be used before their definition, recurse, take local variables as extra parameters, and pass scalars by value and arrays by reference.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.015
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.015
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
p() { awk "$1" 2>/dev/null </dev/null; }
[ "$(p 'function f(a, b) { return a * b } BEGIN { print f(3, 4) }')" = "12" ] || pc_fail "a function call wrong"
[ "$(p 'BEGIN { print g(2) }
function g(x) { return x + 1 }')" = "3" ] || pc_fail "a function used before its definition wrong"
[ "$(p 'function fact(n) { return n <= 1 ? 1 : n * fact(n - 1) }
BEGIN { print fact(5) }')" = "120" ] || pc_fail "recursion wrong"
[ "$(p 'function h(x,   tmp) { tmp = x * 2; return tmp }
BEGIN { tmp = "keep"; print h(4), tmp }')" = "8 keep" ] || pc_fail "an extra parameter was not local"
[ "$(p 'function m(v) { v = 99 } BEGIN { z = 1; m(z); print z }')" = "1" ] || pc_fail "a scalar was not passed by value"
[ "$(p 'function fill(arr) { arr["k"] = "v" } BEGIN { fill(A); print A["k"] }')" = "v" ] || pc_fail "an array was not passed by reference"
[ "$(p 'function r() { return } BEGIN { x = r(); print "[" x "]" }')" = "[]" ] || pc_fail "return with no value wrong"
[ "$(p 'function nr() { } BEGIN { x = nr(); print "[" x "]" }')" = "[]" ] || pc_fail "falling off the end wrong"
[ "$(p 'function k(a, b) { return "[" b "]" } BEGIN { print k(1) }')" = "[]" ] || pc_fail "a missing argument was not uninitialized"
[ "$(p 'function f(x)
{
  return x * 2
}
BEGIN { print f(f(2)) }')" = "8" ] || pc_fail "newlines around the function body or nested calls wrong"
[ "$(p 'function swap(a, i, j,   t) { t = a[i]; a[i] = a[j]; a[j] = t }
BEGIN { x[1] = "p"; x[2] = "q"; swap(x, 1, 2); print x[1] x[2] }')" = "qp" ] || pc_fail "a function modifying an array parameter wrong"
[ "$(p 'function loc(n,   arr) { arr[n] = n; return length(arr) }
BEGIN { print loc(1), loc(2) }')" = "1 1" ] || pc_fail "a local array was not fresh on each call"
pc_pass
