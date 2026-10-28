# A -f after -i cancels it: the file is removed without regard to a negative reply.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.011
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.011
printf x >victim || pc_fail "cannot create victim"
pc_test
printf 'n\n' | rm -i -f victim >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "rm exited with status $rc"
[ ! -e victim ] || pc_fail "victim was not removed"
pc_pass
