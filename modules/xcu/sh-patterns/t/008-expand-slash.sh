# In pathname expansion a slash is matched only by a slash in the pattern,
# never by *, ? or a bracket expression, and a [ with a slash before its ]
# is an ordinary character.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.008
mkdir pc.fe pc.fe/d1 pc.fe/d2 pc.fe/ax || pc_unresolved "mkdir"
: >pc.fe/d1/f
: >pc.fe/d2/f
: >pc.fe/ax/b
: >pc.fe/abd
mkdir pc.fe/a || pc_unresolved "mkdir"
: >pc.fe/a/d
LC_ALL=POSIX
export LC_ALL
pc_test
r=$(cd pc.fe && echo */f)
[ "$r" = 'd1/f d2/f' ] || pc_fail "got '$r' for */f"
r=$(cd pc.fe && echo a*b)
[ "$r" = 'a*b' ] || pc_fail "got '$r' for a*b (a/b must not match)"
r=$(cd pc.fe && echo ax?b)
[ "$r" = 'ax?b' ] || pc_fail "got '$r' for ax?b"
r=$(cd pc.fe && echo 'a[b/c]d')
[ "$r" = 'a[b/c]d' ] || pc_fail "got '$r' for a[b/c]d, expected the pattern unchanged"
pc_pass
