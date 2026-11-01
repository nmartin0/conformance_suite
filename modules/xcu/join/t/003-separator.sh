# join -t makes the character the only separator of input and output fields, so empty fields count.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.003
# MUTANT    join.t_ignored
# MUTANT    join.out_space
# MUTANT    join.t_collapse
# MUTANT    join.t_blank_split
# MUTANT    join.t_blank_strip
# MUTANT    join.t_trim_key
# MUTANT    join.t_leading_dropped
. "$PC_CORE/pc.sh"
pc_begin XCU.join.003
LC_ALL=POSIX
export LC_ALL
NL='
'
TAB=$(printf '\t')
printf 'a:1:\nb::3\nc d:4\n' >f1 && printf 'a:x\nb:y\nc d:z\n' >f2 || pc_fail "cannot create input"
printf ':a:1\n' >l1 && printf 'a:x\n' >l2 || pc_fail "cannot create input"
printf ' a:1\na:2\n' >b1 && printf 'a:x\n' >b2 || pc_fail "cannot create input"
printf 'k\t\t v 1\n' >t1 && printf 'k\tx y\n' >t2 || pc_fail "cannot create input"
pc_test
got=$(join -t : f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:1::x${NL}b::3:y${NL}c d:4:z${NL}rc=0" ] || pc_fail "-t : gave the wrong result"
got=$(join -t : -1 2 -2 1 l1 l2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a::1:x${NL}rc=0" ] || pc_fail "an empty first field was not kept"
got=$(join -t : b1 b2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:2:x${NL}rc=0" ] || pc_fail "a leading blank of the join field was not significant"
got=$(join -t "$TAB" t1 t2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "k${TAB}${TAB} v 1${TAB}x y${NL}rc=0" ] || pc_fail "-t with a tab gave the wrong result"
pc_pass
