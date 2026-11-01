# Exit status 255 of the utility stops xargs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.014
# MUTANT    xargs.s255_continues
# MUTANT    xargs.s255_status0
# MUTANT    xargs.s255_status255
# MUTANT    xargs.s255_no_diag
# MUTANT    xargs.s255_diag_stdout
# MUTANT    xargs.s255_one_more
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.014
LC_ALL=C; export LC_ALL
d=pcxargs014.$$
trap 'rm -rf "$d"' EXIT
mkdir "$d" || pc_fail "cannot make $d"
pc_test
printf 'a b c\n' | xargs -n 1 "${PC_SH:-sh}" -c 'echo "$1" >>"$0"; exit 255' "$d/log" >out 2>err
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "xargs exited with status $rc, expected 1-125"
[ -s err ] || pc_fail "xargs wrote no diagnostic message"
[ -s out ] && pc_fail "xargs wrote to standard output, which it does not use"
n=0
while IFS= read -r l; do
	n=$((n + 1))
done <"$d/log"
[ "$n" -eq 1 ] || pc_fail "the utility ran $n times after exiting with 255, expected 1"
pc_pass
