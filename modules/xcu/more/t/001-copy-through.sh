# output copied unchanged when not a terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.more.001
# When standard output is not a terminal, the input file is copied to it in
# its entirety without modification, and options other than -s (here -c, -e,
# -i, -n, -u) have no effect, even with standard error unreadable.
. "$PC_CORE/pc.sh"
pc_begin XCU.more.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=1
while [ $i -le 120 ]; do
	printf 'line %d\n' $i
	i=$((i + 1))
done >in
pc_test
more in </dev/null >out 2>err || pc_fail "more: nonzero status"
cmp in out >/dev/null 2>&1 || pc_fail "output differs from the input"
more -c -e -i -u -n 3 in </dev/null >out2 2>err || pc_fail "more with options: nonzero status"
cmp in out2 >/dev/null 2>&1 || pc_fail "options changed the copy"
more -n 5 in <in >out3 2>err || pc_fail "more -n 5: nonzero status"
cmp in out3 >/dev/null 2>&1 || pc_fail "-n 5 changed the copy"
pc_pass
