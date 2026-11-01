# A trailing blank makes the next word checked for alias substitution.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.010
# MUTANT    alias.trailing_blank_trimmed
# MUTANT    alias.trailing_blank_always
# MUTANT    alias.trailing_tab_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.010
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
tab=$(printf '\t')
pc_test
alias pcl1='echo '
alias pcl2='there'
alias pcl3='echo'
alias pcl6="echo$tab"
got1=$(use 'pcl1 pcl2')
[ "$got1" = there ] || pc_fail "word after a value with a trailing blank not substituted: '$got1'"
got2=$(use 'pcl3 pcl2')
[ "$got2" = pcl2 ] || pc_fail "word after a value without a trailing blank was substituted: '$got2'"
got3=$(use 'pcl6 pcl2')
[ "$got3" = there ] || pc_fail "word after a value with a trailing tab not substituted: '$got3'"
pc_pass
