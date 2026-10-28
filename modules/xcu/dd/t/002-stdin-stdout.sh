# dd with standard input and standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.002
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Without if= and of= dd reads standard input and writes standard output,
# and nothing but the data goes to standard output.
printf '\000\377\n' >in || pc_fail "cannot write in"
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
gen 0 15 >>in || pc_fail "cannot write in"
pc_test
dd <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd exited with status $rc"
cmp -s in out || pc_fail "standard output differs from standard input"
pc_pass
