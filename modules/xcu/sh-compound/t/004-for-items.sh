# for sets the variable to each word of the expanded list in turn and runs
# the body each time; the variable keeps the last value; the list is
# expanded (parameters, field splitting, pathname, arithmetic,
# substitution).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.004
LC_ALL=POSIX
export LC_ALL
: >pc.f1
: >pc.f2
pc_test
r=
for i in a b c; do r=$r$i; done
[ "$r" = abc ] && [ "$i" = c ] || pc_fail "plain words: r='$r' i='$i'"
pcl='x y'
r=
for i in $pcl; do r=$r[$i]; done
[ "$r" = '[x][y]' ] || pc_fail "field splitting: '$r'"
r=
for i in "$pcl"; do r=$r[$i]; done
[ "$r" = '[x y]' ] || pc_fail "quoted: '$r'"
r=
for i in pc.f*; do r="$r$i,"; done
[ "$r" = 'pc.f1,pc.f2,' ] || pc_fail "pathname expansion: '$r'"
r=
for i in $((1 + 1)) $(printf 'p q'); do r=$r[$i]; done
[ "$r" = '[2][p][q]' ] || pc_fail "arithmetic and substitution: '$r'"
pc_pass
