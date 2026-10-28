# After hash -r the search for a utility starts afresh.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.hash.003
. "$PC_CORE/pc.sh"
pc_begin XCU.hash.003
mkdir d0 d1 || pc_fail "cannot make directories"
printf 'echo one\n' >d1/pchashy || pc_fail "cannot write a script"
chmod +x d1/pchashy || pc_fail "cannot make the script executable"
PATH=$PWD/d0:$PWD/d1:$PATH
pchashy >out1 2>/dev/null
got=$(cat out1)
[ "$got" = one ] || pc_fail "setup: first run gave '$got'"
printf 'echo zero\n' >d0/pchashy || pc_fail "cannot write the second script"
chmod +x d0/pchashy || pc_fail "cannot make the second script executable"
pc_test
hash -r
pchashy >out2 2>/dev/null
got=$(cat out2)
[ "$got" = zero ] || pc_fail "after hash -r the run gave '$got', wanted the copy in the earlier directory (zero)"
pc_pass
