# A command string, a file or standard input that holds only blank lines and
# comments (or nothing) makes sh exit with status zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.006
: >pc.e1
printf '%s\n' '' '# a comment' '   ' '# another' >pc.e2
pc_test
${PC_SH:-sh} -c '' </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "empty command string gave $rc"
${PC_SH:-sh} -c '# only a comment' </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "comment-only command string gave $rc"
${PC_SH:-sh} pc.e1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "empty file gave $rc"
${PC_SH:-sh} pc.e2 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file with blank lines and comments gave $rc"
${PC_SH:-sh} <pc.e2
rc=$?
[ "$rc" -eq 0 ] || pc_fail "standard input with blank lines and comments gave $rc"
pc_pass
