# The alias output is suitable for reinput.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.003
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.003
v=$(cat <<'PCV'
echo 'a b' "c d" \$f '$g'
PCV
)
exp='a b c d $f $g'
pc_test
alias "pcal3=$v"
got1=$(eval pcal3)
[ "$got1" = "$exp" ] || pc_fail "alias with quoting does not run as defined: '$got1'"
alias pcal3 >out 2>/dev/null || pc_fail "alias pcal3 failed"
unalias pcal3
line=$(cat out)
eval "alias $line"
got2=$(eval pcal3)
[ "$got2" = "$exp" ] || pc_fail "reinput of '$line' gives '$got2', wanted '$exp'"
pc_pass
