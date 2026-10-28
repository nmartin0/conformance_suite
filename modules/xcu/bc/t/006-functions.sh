# define creates functions with parameters, auto locals, array parameters and return values; arguments pass by value, redefinition replaces, recursion works, and names refer to the newest binding.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.006
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.006
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
[ "$(b 'define f(x) {\nreturn(x*2)\n}\nf(4)\nf(f(2))')" = "8${NL}8" ] || pc_fail "a simple function wrong"
[ "$(b 'define f(n) {\nif (n<=1) return(1)\nreturn(n*f(n-1))\n}\nf(10)')" = "3628800" ] || pc_fail "recursion wrong"
[ "$(b 'define g(x) {\nauto t\nt=x+1\nreturn(t)\n}\nt=7\ng(1)\nt')" = "2${NL}7" ] || pc_fail "an auto variable was not local"
[ "$(b 'define h(x) {\nx=99\nreturn(x)\n}\ny=1\nh(y)\ny')" = "99${NL}1" ] || pc_fail "an argument was not passed by value"
[ "$(b 'define n(x) {\ny=x\n}\nn(5)')" = "0" ] || pc_fail "a function without return did not give 0"
[ "$(b 'define r() {\nreturn\n}\nr()')" = "0" ] || pc_fail "return with no value did not give 0"
[ "$(b 'define s(a[], n) {\nauto i, t\nfor (i=0; i<n; i++) t+=a[i]\nreturn(t)\n}\na[0]=1\na[1]=2\na[2]=3\ns(a[], 3)')" = "6" ] || pc_fail "an array parameter wrong"
[ "$(b 'define f(x) {\nreturn(1)\n}\ndefine f(x) {\nreturn(2)\n}\nf(0)')" = "2" ] || pc_fail "redefinition did not replace the function"
[ "$(b 'define k(x) {\nreturn(x+z)\n}\nz=10\nk(5)')" = "15" ] || pc_fail "a global variable in a function wrong"
[ "$(b 'define f(x) {\nreturn(x+1)\n}\nf=3\nf[0]=9\nf(f)\nf[0]')" = "4${NL}9" ] || pc_fail "a function, variable and array of one name interfered"
[ "$(b 'define a(x) {\nauto y\ny=x\nreturn(b(y)+y)\n}\ndefine b(x) {\nauto y\ny=100\nreturn(x+y)\n}\na(1)')" = "102" ] || pc_fail "auto variables were not restored after a nested call"
[ "$(b 'define c() {\nreturn(v)\n}\ndefine d(v) {\nreturn(c())\n}\nv=1\nd(5)')" = "5" ] || pc_fail "a called function did not see the caller's parameter"
[ "$(b 'define f(a, b) {\nreturn(a-b)\n}\nf(10, 3)')" = "7" ] || pc_fail "two parameters wrong"
[ "$(b 'define p(x) {\nif (x<0) return(0-x)\nreturn(x)\n}\np(-5)\np(5)')" = "5${NL}5" ] || pc_fail "return from inside an if wrong"
[ "$(b 'define q(x) {\nwhile (1) {\nif (x>3) return(x)\nx=x+1\n}\n}\nq(0)')" = "4" ] || pc_fail "return from inside a loop wrong"
[ "$(b 'define f(x) {\nquit\n}\n1\nf(0)\n2')" = "1" ] || pc_fail "quit inside a function did not stop the program"
pc_pass
