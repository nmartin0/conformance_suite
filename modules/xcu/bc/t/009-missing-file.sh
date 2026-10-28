# When a file operand cannot be read, bc writes a diagnostic and terminates without any further action.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.009
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1+1\n' >fexpr || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
out=$(printf '9+9\n' | bc nosuchfile 2>/dev/null)
[ -z "$out" ] || pc_fail "bc went on to read standard input after a missing file"
out=$(printf '9+9\n' | bc nosuchfile fexpr 2>/dev/null)
[ -z "$out" ] || pc_fail "bc went on to the next file after a missing file"
err=$(printf '1\n' | bc nosuchfile 2>&1 >/dev/null)
[ -n "$err" ] || pc_fail "no diagnostic for a missing file"
pc_pass
