# The value of an assignment gets tilde expansion (also after a colon),
# parameter expansion, command and arithmetic substitution and quote
# removal, but no field splitting and no pathname expansion.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.007
: >pc.g1
HOME=/pc/h
pc_test
PCV=~/x
[ "$PCV" = /pc/h/x ] || pc_fail "tilde: '$PCV'"
PCV=~/a:~/b
[ "$PCV" = /pc/h/a:/pc/h/b ] || pc_fail "tilde after colon: '$PCV'"
PCV=$((1 + 2))
[ "$PCV" = 3 ] || pc_fail "arithmetic: '$PCV'"
PCV=$(printf sub)
[ "$PCV" = sub ] || pc_fail "command substitution: '$PCV'"
pcx='a  b'
PCV=$pcx
[ "$PCV" = 'a  b' ] || pc_fail "field splitting: '$PCV'"
PCV=pc.*
[ "$PCV" = 'pc.*' ] || pc_fail "pathname expansion: '$PCV'"
PCV="a"'b'\c
[ "$PCV" = abc ] || pc_fail "quote removal: '$PCV'"
pc_pass
