# The command string is tokenized and parsed as shell language: reserved
# words, lists and quoting in it work as in a script.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.006
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.006
pc_test
r=$(eval 'if true; then echo yes; else echo no; fi')
[ "$r" = yes ] || pc_fail "if command gave '$r'"
eval 'pca=1; pcb=2'
[ "$pca$pcb" = 12 ] || pc_fail "list gave '$pca$pcb'"
pcv='two words'
r=$(eval 'printf "%s|" "$pcv"')
[ "$r" = 'two words|' ] || pc_fail "quoting gave '$r'"
r=$(eval 'for i in 1 2 3; do printf "%s" "$i"; done')
[ "$r" = 123 ] || pc_fail "for command gave '$r'"
pc_pass
