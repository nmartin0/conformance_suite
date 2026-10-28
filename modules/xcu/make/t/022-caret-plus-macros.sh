# $^ and $+ internal macros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.022
# In a target rule $^ is the list of prerequisites without duplicates and $+
# the list with duplicates.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.022
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
dup: f1 f2 f1
	echo "caret=$^ plus=$+"
EOM
: >f1; : >f2
cat >exp <<'EOM'
caret=f1 f2 plus=f1 f2 f1
EOM
pc_test
make -s dup >o 2>err </dev/null || pc_fail "make: nonzero status"
cmp -s o exp || pc_fail "internal macro values differ: $(cat o)"
pc_pass
