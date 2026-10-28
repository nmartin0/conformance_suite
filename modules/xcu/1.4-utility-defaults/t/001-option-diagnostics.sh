# A standard utility given an option it does not recognize, or an option that needs an argument without one, writes a diagnostic to standard error and exits non-zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rejected=0
pc_test
for u in cat cut head tail sort uniq wc fold paste grep sed ls mkdir rm cp mv touch od; do
	"$u" -% </dev/null >out 2>err
	rc=$?
	[ "$rc" -ne 0 ] || continue
	[ -s err ] || pc_fail "$u -%: exit status $rc and no diagnostic"
	rejected=$((rejected + 1))
done
[ "$rejected" -ge 3 ] || pc_unresolved "only $rejected utilities rejected the probe option"
for c in "head -n" "tail -n" "cut -f" "cut -d" "sort -o" "sort -k" "sed -e" "sed -f" "grep -e" "grep -f" "fold -w" "uniq -f" "uniq -s" "paste -d" "join -t" "split -l"; do
	set -- $c
	"$@" </dev/null >out 2>err
	rc=$?
	[ "$rc" -ne 0 ] || pc_fail "$c: exit status 0 although the option-argument is missing"
	[ -s err ] || pc_fail "$c: no diagnostic for the missing option-argument"
done
pc_pass
