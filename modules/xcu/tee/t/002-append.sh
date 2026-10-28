# Without -a tee overwrites an existing file; with -a it appends to it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tee.002
. "$PC_CORE/pc.sh"
pc_begin XCU.tee.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'new\n' >in || pc_fail "cannot create input"
printf 'old\n' >f1 && printf 'old\n' >f2 || pc_fail "cannot create input"
pc_test
tee f1 <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee f1 gave status $rc"
got=$(cat f1; printf x)
[ "$got" = "new${NL}x" ] || pc_fail "tee without -a did not replace the old contents"
tee -a f2 <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tee -a f2 gave status $rc"
got=$(cat f2; printf x)
[ "$got" = "old${NL}new${NL}x" ] || pc_fail "tee -a did not append"
got=$(cat out; printf x)
[ "$got" = "new${NL}x" ] || pc_fail "tee -a did not copy the input to standard output"
tee -a f3 <in >out 2>/dev/null
got=$(cat f3; printf x)
[ "$got" = "new${NL}x" ] || pc_fail "tee -a did not create a missing file"
pc_pass
