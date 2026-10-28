# prerequisites accumulate; inference rules are redefined
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.031
# Prerequisites of a target given on several lines add up while one line
# supplies the commands, and when an inference rule is defined twice the
# last definition is used.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.031
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
t: a
t: b
t: ; @echo "t:$^"
a: ; @echo a-made
b: ; @echo b-made
EOM
cat >inf.mk <<'EOM'
.POSIX:
.SUFFIXES: .in .out
.in.out:
	echo first-rule
.in.out:
	echo second-rule
EOM
: >x.in || pc_unresolved "setup: cannot make x.in"
pc_test
make t >o 2>err </dev/null || pc_fail "make t: nonzero status"
grep -x a-made o >/dev/null || pc_fail "the prerequisite from the first line was not made"
grep -x b-made o >/dev/null || pc_fail "the prerequisite from the second line was not made"
grep -Fx 't:a b' o >/dev/null || pc_fail "t was not made after both prerequisites: $(cat o)"
make -f inf.mk x.out >o 2>err </dev/null || pc_fail "make x.out: nonzero status"
grep -x second-rule o >/dev/null || pc_fail "the last definition of the inference rule was not used: $(cat o)"
grep first-rule o >/dev/null && pc_fail "the first definition of the inference rule was used"
pc_pass
