# When a utility writes a regular file whose name is an existing FIFO, it waits until the FIFO is open for reading and then opens it for writing and carries on, so the data reaches the reader
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.007
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkfifo p || pc_unresolved "mkfifo"
kids=
cleanup() { for k in $kids; do kill -s KILL "$k" 2>/dev/null; done; }
trap 'cleanup; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
tee p </dev/null >/dev/null 2>&1 &
w=$!; kids="$kids $w"
sleep 2
kill -0 "$w" 2>/dev/null || pc_fail "tee finished although no process had the FIFO open for reading"
cat p >got1 &
r=$!; kids="$kids $r"
i=0
while kill -0 "$w" 2>/dev/null && [ "$i" -lt 10 ]; do
	sleep 1
	i=$((i + 1))
done
kill -0 "$w" 2>/dev/null && pc_fail "tee did not carry on after a reader opened the FIFO"
printf 'hello\n' | tee p >/dev/null 2>&1 &
w2=$!; kids="$kids $w2"
cat p >got2 &
r2=$!; kids="$kids $r2"
i=0
while kill -0 "$r2" 2>/dev/null && [ "$i" -lt 10 ]; do
	sleep 1
	i=$((i + 1))
done
kill -0 "$r2" 2>/dev/null && pc_fail "the reader never saw end-of-file"
[ "$(cat got2)" = hello ] || pc_fail "the data written to the FIFO did not arrive: '$(cat got2)'"
pc_pass
