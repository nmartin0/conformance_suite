# In a pipeline the standard output of each command is the standard input of
# the next.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.001
pc_test
r=$(printf 'a\nb\n' | { read x; read y; echo "$y$x"; })
[ "$r" = ba ] || pc_fail "two stages: '$r'"
r=$(echo x | cat | cat | cat)
[ "$r" = x ] || pc_fail "four stages: '$r'"
pc_pass
