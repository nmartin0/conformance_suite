# Redirections of a command override the pipe connections made by the
# pipeline: an input redirection replaces the pipe as input, an output
# redirection leaves the next command an empty input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.007
printf 'file\n' >pc.p1 || pc_unresolved "write"
pc_test
r=$(echo pipe | cat <pc.p1)
[ "$r" = file ] || pc_fail "input redirection: '$r'"
r=$(echo hello >pc.p3 | cat)
[ -z "$r" ] || pc_fail "output redirection: next command read '$r'"
[ "$(cat pc.p3)" = hello ] || pc_fail "the redirected output is '$(cat pc.p3)'"
pc_pass
