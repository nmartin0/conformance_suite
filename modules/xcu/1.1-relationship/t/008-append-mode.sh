# When a utility appends to a file it opens it for appending without truncating: tee -a keeps the old contents, creates a missing file, and writes at the current end of a file that another process has meanwhile emptied
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.008
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkfifo ctl || pc_unresolved "mkfifo"
printf 'OLDOLDOLDOLD\n' >f || pc_unresolved "write"
printf 'a\n' >g || pc_unresolved "write"
kids=
cleanup() { for k in $kids; do kill -s KILL "$k" 2>/dev/null; done; }
trap 'cleanup; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
printf 'b\n' | tee -a g >/dev/null || pc_fail "tee -a failed"
printf 'a\nb\n' >want.g
cmp -s g want.g || pc_fail "tee -a did not keep the old contents: $(tr '\n' '|' <g)"
printf 'n\n' | tee -a newfile >/dev/null || pc_fail "tee -a failed on a missing file"
[ "$(cat newfile 2>/dev/null)" = n ] || pc_fail "tee -a did not create the missing file"
tee -a f <ctl >/dev/null 2>&1 &
tp=$!; kids="$kids $tp"
exec 4>ctl
sleep 2
: >f
printf 'x\n' >&4
exec 4>&-
i=0
while kill -0 "$tp" 2>/dev/null && [ "$i" -lt 10 ]; do
	sleep 1
	i=$((i + 1))
done
kill -0 "$tp" 2>/dev/null && pc_fail "tee -a did not end at end-of-file"
printf 'x\n' >want.f
cmp -s f want.f || pc_fail "tee -a did not write at the end of the emptied file (size $(wc -c <f | tr -d ' '))"
pc_pass
