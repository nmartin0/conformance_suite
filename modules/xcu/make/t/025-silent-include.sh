# -include
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.025
# A missing file named on an -include line is ignored, and a file that
# exists is still read.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.025
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
-include missing.mk
-include here.mk
all:
	echo "[$(FROM_HERE)]"
EOM
printf 'FROM_HERE = yes\n' >here.mk
pc_test
make -s >o 2>err </dev/null || pc_fail "make with a missing -include file: nonzero status"
[ "$(cat o)" = '[yes]' ] || pc_fail "-include gave '$(cat o)'"
pc_pass
