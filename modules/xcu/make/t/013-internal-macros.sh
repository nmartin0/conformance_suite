# $@ $? and D/F forms
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.013
# In a target rule $@ is the target, $? only the prerequisites newer than the
# target, and $(@D) and $(@F) the directory part (. when there is none) and
# the file name part.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
prog: f1 f2 f3
	echo "T1 at=$@ q=$?"
sub/x.out: f1
	echo "T3 $(@D) $(@F)"
cur: f1
	echo "T4 $(@D) $(@F)"
EOM
mkdir sub || pc_unresolved "setup: mkdir failed"
: >f1; : >f2; : >f3; : >prog
touch -t 200001010000 f1 || pc_unresolved "setup: touch failed"
touch -t 200101010000 prog || pc_unresolved "setup: touch failed"
cat >exp <<'EOM'
T1 at=prog q=f2 f3
T3 sub x.out
T4 . cur
EOM
pc_test
make -s prog sub/x.out cur >o 2>err </dev/null || pc_fail "make: nonzero status"
cmp -s o exp || pc_fail "internal macro values differ: $(cat o)"
pc_pass
