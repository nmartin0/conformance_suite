# Pathname expansion replaces a pattern that matches existing names by those
# names, sorted by the collating sequence (byte order in the POSIX locale).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.006
LC_ALL=POSIX
export LC_ALL
mkdir pc.fe || pc_unresolved "mkdir"
for n in b a c A B; do : >pc.fe/$n; done
pc_test
r=$(cd pc.fe && echo *)
[ "$r" = 'A B a b c' ] || pc_fail "got '$r', expected 'A B a b c'"
r=$(cd pc.fe && echo [ab]*)
[ "$r" = 'a b' ] || pc_fail "got '$r' for [ab]*, expected 'a b'"
r=$(cd pc.fe && echo ?)
[ "$r" = 'A B a b c' ] || pc_fail "got '$r' for ?, expected 'A B a b c'"
pc_pass
