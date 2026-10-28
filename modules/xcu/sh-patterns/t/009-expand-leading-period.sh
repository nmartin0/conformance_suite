# A leading period in a name is matched only by an explicit period in the
# pattern (first, or just after a slash), not by *, ?, [!a], or a range.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.009
mkdir pc.fe pc.fe/pc.dd || pc_unresolved "mkdir"
: >pc.fe/.hid
: >pc.fe/vis
: >pc.fe/pc.dd/.in
: >pc.fe/pc.dd/out
LC_ALL=POSIX
export LC_ALL
pc_test
r=$(cd pc.fe && echo *)
case " $r " in
*" .hid "*) pc_fail "* matched .hid ('$r')" ;;
esac
r=$(cd pc.fe && echo ?hid)
[ "$r" = '?hid' ] || pc_fail "? matched the leading period ('$r')"
r=$(cd pc.fe && echo [!a]hid)
[ "$r" = '[!a]hid' ] || pc_fail "[!a] matched the leading period ('$r')"
r=$(cd pc.fe && echo [%-0]hid)
[ "$r" = '[%-0]hid' ] || pc_fail "a range matched the leading period ('$r')"
r=$(cd pc.fe && echo .h*)
[ "$r" = .hid ] || pc_fail "explicit period: got '$r', expected .hid"
r=$(cd pc.fe && echo pc.dd/*)
[ "$r" = pc.dd/out ] || pc_fail "after a slash: got '$r', expected pc.dd/out"
r=$(cd pc.fe && echo pc.dd/.i*)
[ "$r" = pc.dd/.in ] || pc_fail "explicit period after a slash: got '$r', expected pc.dd/.in"
pc_pass
