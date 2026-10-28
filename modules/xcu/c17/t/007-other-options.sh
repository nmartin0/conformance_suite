# c17 -O, -g, -s
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.007
# "-O 0", "-O 1", "-g" and "-s" are each accepted and the resulting program
# runs with its normal output and exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '#include <stdio.h>\nint main(void) { puts("ok"); return 5; }\n' >p.c
pc_test
for opt in "-O 0" "-O 1" "-g" "-s"; do
	rm -f p
	c17 $opt -o p p.c >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "c17 $opt: exit status $rc"
	./p >o </dev/null
	rc=$?
	[ "$rc" -eq 5 ] || pc_fail "program built with $opt exits with $rc, expected 5"
	[ "$(cat o)" = ok ] || pc_fail "program built with $opt prints '$(cat o)'"
done
pc_pass
