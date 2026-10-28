# make rebuilds only out-of-date targets
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.003
# A target older than a prerequisite is rebuilt; a target newer than its
# prerequisites is not; prerequisites are brought up to date in the order
# listed before the target's own commands; a missing target is built.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
tgt: src
	cp src tgt
top: p1 p2
	echo top
p1:
	echo p1
p2:
	echo p2
EOM
printf 'old\n' >tgt; printf 's1\n' >src
touch -t 200001010000 tgt || pc_unresolved "setup: touch failed"
touch -t 200101010000 src || pc_unresolved "setup: touch failed"
pc_test
make tgt >o 2>err </dev/null || pc_fail "make tgt: nonzero status"
[ "$(cat tgt)" = s1 ] || pc_fail "the out-of-date target was not rebuilt"
printf 's2\n' >src
touch -t 200001010000 src || pc_unresolved "touch failed"
make tgt >o 2>err </dev/null || pc_fail "make tgt (up to date): nonzero status"
[ "$(cat tgt)" = s1 ] || pc_fail "an up-to-date target was rebuilt"
rm -f tgt
make tgt >o 2>err </dev/null || pc_fail "make tgt (missing): nonzero status"
[ "$(cat tgt)" = s2 ] || pc_fail "a missing target was not built"
make -s top >o 2>err </dev/null || pc_fail "make top: nonzero status"
got=$(grep -x -e p1 -e p2 -e top o | tr '\n' ' ')
[ "$got" = 'p1 p2 top ' ] || pc_fail "prerequisites were processed in order '$got'"
pc_pass
