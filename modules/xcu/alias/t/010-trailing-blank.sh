# A trailing blank makes the next word checked for alias substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.010
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.010
pc_test
alias pcl1='echo '
alias pcl2='there'
alias pcl3='echo'
got1=$(eval 'pcl1 pcl2')
[ "$got1" = there ] || pc_fail "word after a value with a trailing blank not substituted: '$got1'"
got2=$(eval 'pcl3 pcl2')
[ "$got2" = pcl2 ] || pc_fail "word after a value without a trailing blank was substituted: '$got2'"
pc_pass
