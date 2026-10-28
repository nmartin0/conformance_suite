# A utility opens a file to read it with read access only and to write it with write access only: a file the user can only read can be read by cat, sort, wc, head, cksum, od, grep, sed, cut and awk, and a file the user can only write can be written by tee, tee -a, a redirection, dd, sort -o, cp and sed w
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.009
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >r1 || pc_unresolved "write"
chmod 400 r1 || pc_unresolved "chmod"
printf 'old\n' >w1 || pc_unresolved "write"
chmod 200 w1 || pc_unresolved "chmod"
pc_test
for c in "cat r1" "sort r1" "wc -l r1" "head -n 1 r1" "cksum r1" "od r1" "grep x r1" "sed -n p r1" "cut -c 1 r1" "awk '{ print }' r1"; do
	eval "$c" >o 2>e </dev/null || pc_fail "'$c' could not read a file with only read permission"
done
for c in "tee w1" "tee -a w1" ": >w1" "dd if=r1 of=w1" "sort -o w1 r1" "cp r1 w1" "sed -n 'w w1' r1"; do
	chmod 200 w1 || pc_unresolved "chmod"
	eval "$c" >o 2>e </dev/null || pc_fail "'$c' could not write a file with only write permission"
done
chmod 600 w1 || pc_unresolved "chmod"
pc_pass
