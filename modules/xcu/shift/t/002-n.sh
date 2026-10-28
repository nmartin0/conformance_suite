# shift n drops the first n positional parameters; shift 0 changes nothing;
# shifting all of them leaves none and succeeds.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.shift.002
. "$PC_CORE/pc.sh"
pc_begin XCU.shift.002
pc_test
set a b c d e
shift 2
rc=$?
[ "$rc" -eq 0 ] && [ "$#" -eq 3 ] && [ "$1$2$3" = cde ] || pc_fail "shift 2: status $rc, \$#=$#, params '$*'"
shift 0
rc=$?
[ "$rc" -eq 0 ] && [ "$#" -eq 3 ] && [ "$1$2$3" = cde ] || pc_fail "shift 0: status $rc, \$#=$#, params '$*'"
shift 3
rc=$?
[ "$rc" -eq 0 ] && [ "$#" -eq 0 ] && [ -z "${1+set}" ] || pc_fail "shift 3: status $rc, \$#=$#"
pc_pass
