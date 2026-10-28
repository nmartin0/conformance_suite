# macro assignment forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.011
# "=" is delayed, "::=" and ":::=" expand at definition, "!=" takes the
# shell output (newlines become spaces, failure is not an error), "?="
# assigns only if not defined, "+=" appends with a space.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
A = 1
B = $(A)
C ::= $(A)
D :::= $(A)
E != echo shellvalue
F ?= first
F ?= second
G = one
G += two
H ::= a
I != printf 'a\nb\n'
J != false
A = 2
H += $(A)
all:
	echo "B=$(B) C=$(C) D=$(D) E=$(E) F=$(F) G=$(G) H=$(H) I=$(I) J=[$(J)]"
EOM
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
want='B=2 C=1 D=1 E=shellvalue F=first G=one two H=a 2 I=a b J=[]'
[ "$(cat o)" = "$want" ] || pc_fail "macro values are '$(cat o)', expected '$want'"
pc_pass
