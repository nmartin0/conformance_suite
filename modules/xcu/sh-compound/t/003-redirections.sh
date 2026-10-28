# A redirection after a compound command applies to every command inside it
# that does not redirect that descriptor itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.003
printf 'x\ny\n' >pc.in3 || pc_unresolved "write"
pc_test
nl='
'
{ echo a; echo b; } >pc.g1
[ "$(cat pc.g1)" = "a${nl}b" ] || pc_fail "brace group: '$(cat pc.g1)'"
{ echo c; echo d >pc.g3; } >pc.g2
[ "$(cat pc.g2)" = c ] || pc_fail "inner override: outer file holds '$(cat pc.g2)'"
[ "$(cat pc.g3)" = d ] || pc_fail "inner override: inner file holds '$(cat pc.g3)'"
( echo e; echo f ) >pc.g4
[ "$(cat pc.g4)" = "e${nl}f" ] || pc_fail "subshell: '$(cat pc.g4)'"
for i in 1 2; do echo "$i"; done >pc.g5
[ "$(cat pc.g5)" = "1${nl}2" ] || pc_fail "for: '$(cat pc.g5)'"
while read l; do echo "<$l>"; done <pc.in3 >pc.g6
[ "$(cat pc.g6)" = "<x>${nl}<y>" ] || pc_fail "while with input: '$(cat pc.g6)'"
if true; then echo yes; fi >pc.g7
[ "$(cat pc.g7)" = yes ] || pc_fail "if: '$(cat pc.g7)'"
case x in x) echo cs ;; esac >pc.g8
[ "$(cat pc.g8)" = cs ] || pc_fail "case: '$(cat pc.g8)'"
pc_pass
