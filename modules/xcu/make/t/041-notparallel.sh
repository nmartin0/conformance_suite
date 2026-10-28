# .NOTPARALLEL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.041
# With .NOTPARALLEL in the makefile make updates one target at a time even
# when -j asks for more.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.041
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
.NOTPARALLEL:
all: a b c
a b c:
	@echo start $@ >>log; sleep 1; echo end $@ >>log
EOM
pc_test
make -j 4 >o 2>err </dev/null || pc_fail "make -j 4: nonzero status"
[ "$(wc -l <log | tr -d ' ')" -eq 6 ] || pc_fail "the log does not hold the three runs: $(cat log)"
awk '{ if (NR % 2 == 1) { if ($1 != "start") bad = 1; name = $2 } else if ($1 != "end" || $2 != name) bad = 1 } END { exit bad }' log || pc_fail "targets overlapped under .NOTPARALLEL: $(cat log)"
pc_pass
