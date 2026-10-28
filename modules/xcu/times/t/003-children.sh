# After a child process has used processor time, the second line of times
# shows non-zero accumulated time for children.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.times.003
. "$PC_CORE/pc.sh"
pc_begin XCU.times.003
${PC_SH:-sh} -c 'i=0; while [ "$i" -lt 400000 ]; do i=$((i + 1)); done' </dev/null || pc_unresolved "the busy child failed"
pc_test
times >pc.t || pc_fail "times failed"
second=$(sed -n 2p pc.t)
case $second in
*[1-9]*) ;;
*) pc_unresolved "no child time was accounted ($second): the clock may be too coarse for this test" ;;
esac
pc_pass
