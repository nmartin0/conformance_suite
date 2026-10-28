# .PHONY
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.024
# A target named in .PHONY always runs even if a file of that name exists.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.024
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >phony.mk <<'EOM'
.POSIX:
.PHONY: clean
clean:
	echo cleaning
EOM
: >clean
pc_test
make -s -f phony.mk clean >o 2>err </dev/null || pc_fail ".PHONY: nonzero status"
grep -x cleaning o >/dev/null || pc_fail "a phony target did not run although a file of that name exists"
pc_pass
