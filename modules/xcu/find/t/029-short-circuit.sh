# find: -a and -o short-circuit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.029
. "$PC_CORE/pc.sh"
pc_begin XCU.find.029
printf 'x\n' >f || pc_unresolved "cannot create f"
pc_test
: >log
find f -name nomatch -a -exec sh -c 'echo ran >>log' \; >out 2>err </dev/null || pc_fail "find -a: nonzero status: $(cat err)"
[ -s log ] && pc_fail "the right operand of -a ran although the left was false"
find f -name f -o -exec sh -c 'echo ran >>log' \; >out 2>err </dev/null || pc_fail "find -o: nonzero status: $(cat err)"
[ -s log ] && pc_fail "the right operand of -o ran although the left was true"
find f -name f -a -exec sh -c 'echo ran >>log' \; >out 2>err </dev/null || pc_fail "find -a true: nonzero status"
[ -s log ] || pc_fail "the right operand of -a did not run although the left was true"
: >log
find f -name nomatch -o -exec sh -c 'echo ran >>log' \; >out 2>err </dev/null || pc_fail "find -o false: nonzero status"
[ -s log ] || pc_fail "the right operand of -o did not run although the left was false"
pc_pass
