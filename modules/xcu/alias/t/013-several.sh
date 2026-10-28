# Several operands in one call are all processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.013
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.013
pc_test
alias pcm1='echo m1' pcm2='echo m2'
rc=$?
[ "$rc" -eq 0 ] || pc_fail "alias with two definitions exited $rc"
g1=$(eval pcm1)
g2=$(eval pcm2)
[ "$g1" = m1 ] || pc_fail "first alias gives '$g1'"
[ "$g2" = m2 ] || pc_fail "second alias gives '$g2'"
alias pcm1 pcm2 >out 2>/dev/null || pc_fail "alias with two names failed"
{
	IFS= read -r l1
	IFS= read -r l2
} <out
case $l1 in pcm1=*) ;; *) pc_fail "first line is '$l1'" ;; esac
case $l2 in pcm2=*) ;; *) pc_fail "second line is '$l2'" ;; esac
pc_pass
