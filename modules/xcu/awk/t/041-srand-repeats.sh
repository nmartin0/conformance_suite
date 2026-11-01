# srand with the same seed repeats the sequence of rand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.041
# MUTANT    awk.srand_noop
# MUTANT    awk.srand_seed_mixed
# VARIANT    awk.srand_initial_seed
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.041
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'srand(1); a = rand(); srand(1); b = rand(); print (a == b), (a >= 0 && a < 1)')" = "1 1" ] || pc_fail "srand did not repeat the sequence or rand was out of range"
[ "$(e 'srand(7); a = rand(); b = rand(); c = rand(); srand(7); print (a == rand()), (b == rand()), (c == rand())')" = "1 1 1" ] || pc_fail "srand with the same seed did not repeat a longer sequence"
pc_pass
