# pax: a directory loop is detected and ends the run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.024
# "pax -w -L d" on a hierarchy holding a symbolic link to an ancestor
# directory stops (does not run on) and writes a diagnostic to standard
# error.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.024
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
pc_pid=
trap '[ -n "$pc_pid" ] && kill -9 "$pc_pid" 2>/dev/null; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir -p d/e || pc_unresolved "setup: mkdir failed"
printf 'x\n' >d/e/f || pc_unresolved "setup: cannot write d/e/f"
ln -s .. d/e/up || pc_unresolved "setup: ln -s failed"
pc_test
# A limit on the file size keeps a pax that does not stop from filling the disk.
( ulimit -f 4096 2>/dev/null; exec pax -w -L -f a d ) >o 2>err </dev/null &
pc_pid=$!
i=0
while kill -0 "$pc_pid" 2>/dev/null && [ "$i" -lt 30 ]; do
	sleep 1
	i=$((i + 1))
done
if kill -0 "$pc_pid" 2>/dev/null; then
	kill -9 "$pc_pid" 2>/dev/null
	wait "$pc_pid" 2>/dev/null
	pc_pid=
	pc_fail "pax -w -L still ran 30 seconds after entering the directory loop"
fi
wait "$pc_pid" 2>/dev/null
pc_pid=
[ -s err ] || pc_fail "pax stopped (or was stopped) without writing a diagnostic to standard error"
sz=$(wc -c <a); sz=$((sz + 0))
[ "$sz" -lt 2000000 ] || pc_fail "the archive grew to $sz octets: the loop was not detected"
pc_pass
