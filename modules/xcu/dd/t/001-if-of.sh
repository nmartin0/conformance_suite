# dd if= of= copies the file and truncates a longer output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.001
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# if=file and of=file copy the input file byte for byte with the default
# block sizes; a longer existing output file is truncated.
printf '\000\377\n' >in || pc_fail "cannot write in"
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
gen 0 15 >>in || pc_fail "cannot write in"
gen 0 40 >big || pc_fail "cannot write big"
pc_test
dd if=in of=out </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd exited with status $rc"
cmp -s in out || pc_fail "the copy differs from the input"
cat big >out2 || pc_fail "cannot write out2"
dd if=in of=out2 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd onto an existing file exited with status $rc"
cmp -s in out2 || pc_fail "an existing longer output file was not truncated to the copied data"
pc_pass
