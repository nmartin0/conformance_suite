# A word with quoting is not substituted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.011
# MUTANT    alias.quoted_bs
# MUTANT    alias.quoted_sq
# MUTANT    alias.quoted_dq
# MUTANT    alias.quoted_part_bs
# MUTANT    alias.quoted_part_dq
# MUTANT    alias.quoted_part_sq
#
# A function of the same name shows which of the two ran: the alias prints
# "aliased", the function "real". A quoting character anywhere in the word,
# whole or in part, keeps the alias away.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.011
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
pcq11() { echo real; }
pc_test
alias pcq11='echo aliased'
got=$(use pcq11 2>/dev/null)
[ "$got" = aliased ] || pc_fail "unquoted word not substituted: '$got'"
for w in '\pcq11' "'pcq11'" '"pcq11"' 'pc\q11' 'p"cq"11' "pcq1'1'"; do
	got=$(use "$w" 2>/dev/null)
	[ "$got" != aliased ] || pc_fail "the quoted word $w was substituted"
	[ "$got" = real ] || pc_fail "the quoted word $w ran '$got', wanted the function"
done
pc_pass
