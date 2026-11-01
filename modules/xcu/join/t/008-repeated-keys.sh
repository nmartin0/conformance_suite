# When a key occurs several times, join writes every combination of the other fields, in the order the lines were met.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.008
# MUTANT    join.rep_zip
# MUTANT    join.rep_swapped_order
# MUTANT    join.rep_reverse
# MUTANT    join.rep_f2_first_only
# MUTANT    join.rep_f2_last_only
# MUTANT    join.rep_f1_first_only
# MUTANT    join.rep_f1_last_only
. "$PC_CORE/pc.sh"
pc_begin XCU.join.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a 1\na 2\nb 3\n' >f1 && printf 'a x\na y\nb z\n' >f2 || pc_fail "cannot create input"
printf 'a 1\na 2\na 3\nb 4\n' >g1 && printf 'a x\na y\nb z\n' >g2 || pc_fail "cannot create input"
printf 'a x\na y\na z\n' >h1 && printf 'a p\n' >h2 || pc_fail "cannot create input"
printf 'a p\n' >k1 && printf 'a x\na y\na z\n' >k2 || pc_fail "cannot create input"
pc_test
got=$(join f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}a 1 y${NL}a 2 x${NL}a 2 y${NL}b 3 z${NL}rc=0" ] || pc_fail "repeated keys did not give all combinations in order"
got=$(join g1 g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a 1 x${NL}a 1 y${NL}a 2 x${NL}a 2 y${NL}a 3 x${NL}a 3 y${NL}b 4 z${NL}rc=0" ] || pc_fail "three lines against two did not give all combinations in order"
got=$(join h1 h2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a x p${NL}a y p${NL}a z p${NL}rc=0" ] || pc_fail "a repeated key in file1 only did not give every line"
got=$(join k1 k2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a p x${NL}a p y${NL}a p z${NL}rc=0" ] || pc_fail "a repeated key in file2 only did not give every line"
pc_pass
