# -j 1 and repeated -j
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.042
# With -j 1 targets are updated one at a time, and when -j is given several
# times the last value is used.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.042
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
all: a b c
a b c:
	@echo start $@ >>log; sleep 1; echo end $@ >>log
EOM
serial() {
	[ "$(wc -l <log | tr -d ' ')" -eq 6 ] || return 1
	awk '{ if (NR % 2 == 1) { if ($1 != "start") bad = 1; name = $2 } else if ($1 != "end" || $2 != name) bad = 1 } END { exit bad }' log
}
pc_test
make -j 1 >o 2>err </dev/null || pc_fail "make -j 1: nonzero status"
serial || pc_fail "targets overlapped with -j 1: $(cat log)"
rm -f log a b c
make -j 4 -j 1 >o 2>err </dev/null || pc_fail "make -j 4 -j 1: nonzero status"
serial || pc_fail "targets overlapped with -j 4 -j 1: $(cat log)"
pc_pass
