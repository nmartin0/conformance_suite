# A file operand that cannot be read gives a nonzero status, and the other operands are still counted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.009
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a b\n' >g || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(wc -l nosuchfile g 2>/dev/null </dev/null)
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although a file could not be read"
case " $out " in
*" 1 g "*) ;;
*) pc_fail "the readable file was not counted" ;;
esac
pc_pass
