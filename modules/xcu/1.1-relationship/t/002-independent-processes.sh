# A process started by another keeps running when its creator exits: a background subshell started by a shell that then exits still completes its work
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rm -f marker
pc_test
${PC_SH:-sh} -c '(sleep 2; echo done >marker) </dev/null >/dev/null 2>&1 & exit 0' || pc_fail "the creating shell failed"
i=0
while [ ! -s marker ] && [ "$i" -lt 20 ]; do
	sleep 1
	i=$((i + 1))
done
[ "$(cat marker 2>/dev/null)" = done ] || pc_fail "the background process did not complete after its creator exited"
pc_pass
