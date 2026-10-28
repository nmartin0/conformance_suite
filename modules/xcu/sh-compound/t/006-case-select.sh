# case runs the compound list of the first clause whose pattern matches
# (patterns separated by | are alternatives, a leading ( is allowed), and
# only that one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.006
pc_test
case abc in a*) r=1 ;; abc) r=2 ;; esac
[ "$r" = 1 ] || pc_fail "first match: '$r'"
case b in a|b|c) r=alt ;; *) r=no ;; esac
[ "$r" = alt ] || pc_fail "alternatives: '$r'"
case b in (a|b) r=paren ;; esac
[ "$r" = paren ] || pc_fail "leading parenthesis: '$r'"
case zz in a) r=a ;; b) r=b ;; *) r=default ;; esac
[ "$r" = default ] || pc_fail "default clause: '$r'"
case x in x) r=last ;; esac
[ "$r" = last ] || pc_fail "last clause without terminator: '$r'"
pc_pass
