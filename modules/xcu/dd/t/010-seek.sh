# dd seek= keeps skipped blocks only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.010
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# With seek=n and no notrunc the n skipped blocks are kept, nothing after
# the written data is; an old longer file is shortened.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
rep() { r=0; while [ "$r" -lt "$1" ]; do printf '%s' "$2"; r=$((r+1)); done; }
gen 0 5 >old || pc_fail "cannot write old"
rep 50 X >new || pc_fail "cannot write new"
{ gen 0 2; rep 50 X; } >exp || pc_fail "cannot write exp"
pc_test
cat old >out || pc_fail "cannot write out"
dd if=new of=out obs=100 seek=2 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd obs=100 seek=2 exited with status $rc"
cmp -s exp out || pc_fail "obs=100 seek=2: the output is not the first 200 old bytes plus the new data"
cat old >out || pc_fail "cannot write out"
dd if=new of=out bs=100 seek=2 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd bs=100 seek=2 exited with status $rc"
cmp -s exp out || pc_fail "bs=100 seek=2: the output is not the first 200 old bytes plus the new data"
pc_pass
