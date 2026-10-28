# Blank/newline separation, quotes, backslash.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.002
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.002
LC_ALL=C; export LC_ALL
pc_test
# a  b / empty line / 'c d' "e f" g\ h
printf 'a  b\n\n\047c d\047 \042e f\042 g\134 h\n' | xargs printf '[%s]' >out 2>/dev/null || pc_fail "xargs failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "[a][b][c d][e f][g h]" ] || pc_fail "got '$v', expected '[a][b][c d][e f][g h]'"
# "x'y" 'p"q' a\"b
printf '\042x\047y\042 \047p\042q\047 a\134\042b\n' | xargs printf '[%s]' >out 2>/dev/null || pc_fail "xargs failed on quotes inside quotes"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "[x'y][p\"q][a\"b]" ] || pc_fail "got '$v', expected [x'y][p\"q][a\"b]"
pc_pass
