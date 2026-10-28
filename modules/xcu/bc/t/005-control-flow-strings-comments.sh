# if, while, for, break and quit work as described, relational operators can be used inside them, strings are written as given, and comments have no effect.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.005
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.005
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
[ "$(b 'i=0\nwhile (i<3) {\ni\ni=i+1\n}')" = "0${NL}1${NL}2" ] || pc_fail "while wrong"
[ "$(b 'for (i=1; i<=3; i++) i')" = "1${NL}2${NL}3" ] || pc_fail "for wrong"
[ "$(b 'if (1<2) 5')" = "5" ] || pc_fail "if with a true relation did not run the statement"
[ "$(b 'if (2<1) 5')" = "" ] || pc_fail "if with a false relation ran the statement"
[ "$(b 'if (1<2) 1\nif (2<1) 2\nif (2<=2) 3\nif (3>=4) 4\nif (1==1) 5\nif (1!=1) 6\nif (1!=2) 7\nif (3>2) 8')" = "1${NL}3${NL}5${NL}7${NL}8" ] || pc_fail "a relational operator gave the wrong truth value"
[ "$(b 'if (1.5>1.25) 1\nif (-1<0) 2')" = "1${NL}2" ] || pc_fail "comparison of fractions or negatives wrong"
[ "$(b 'for (i=0; i<10; i++) {\nif (i==3) break\ni\n}')" = "0${NL}1${NL}2" ] || pc_fail "break wrong"
[ "$(b 'for (i=1; i<=2; i++) {\nfor (j=1; j<=2; j++) {\ni*10+j\n}\n}')" = "11${NL}12${NL}21${NL}22" ] || pc_fail "nested loops wrong"
[ "$(b 'i=5\nwhile (i>0) i=i-2\ni')" = "-1" ] || pc_fail "while with a simple body wrong"
[ "$(b '1\nquit\n2')" = "1" ] || pc_fail "quit did not stop execution"
# the text can be read as quit taking effect when it is executed (the loop prints 0 and 1 and stops) or when it is read (nothing of the loop runs)
got=$(b 'for (i=0; i<5; i++) {\nif (i==2) quit\ni\n}\n99')
case $got in
"" | "0${NL}1") ;;
*) pc_fail "quit inside a loop did not stop execution: [$got]" ;;
esac
got=$(printf '"hello"\n' | bc 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello${NL}rc=0" ] || [ "$got" = "hellorc=0" ] || pc_fail "a string was not written as given"
got=$(printf '"x = "\n5\n' | bc 2>/dev/null)
[ "$got" = "x = 5" ] || pc_fail "a string followed by an expression wrong"
got=$(printf '"a"; "b"\n' | bc 2>/dev/null; printf '|')
case $got in
"ab|" | "ab${NL}|") ;;
*) pc_fail "two strings were not written one after the other" ;;
esac
[ "$(b '/* comment */ 1+1')" = "2" ] || pc_fail "a comment changed the result"
[ "$(b '1 /* x\ny */ + 2')" = "3" ] || pc_fail "a multi-line comment changed the result"
[ "$(b '1;2;3')" = "1${NL}2${NL}3" ] || pc_fail "semicolon-separated statements wrong"
[ "$(b 'x=1; y=2; x+y')" = "3" ] || pc_fail "assignments separated by semicolons wrong"
pc_pass
