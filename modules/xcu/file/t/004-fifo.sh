# A FIFO is described with the word fifo.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.file.004
. "$PC_CORE/pc.sh"
pc_begin XCU.file.004
mkfifo p || pc_fail "cannot create FIFO"
pc_test
file p >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file exited with status $rc"
l=$(cat out)
case $l in
"p: "*fifo*) ;;
*) pc_fail "line is: $l" ;;
esac
pc_pass
