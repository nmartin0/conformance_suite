# l folds long lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.019
# The l command folds a long line, marking each fold with a backslash and a
# newline, and ends the output with a dollar sign.
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.019
LC_ALL=POSIX
export LC_ALL
NL='
'
mkx() { s=x; while [ "${#s}" -lt "$1" ]; do s=$s$s; done; printf "%.${1}s" "$s"; }
mkx 5000 >big || pc_fail "cannot create input"
printf '\n' >>big
pc_test
sed -n l big >o 2>/dev/null </dev/null || pc_fail "sed -n l: nonzero status"
lines=$(wc -l <o | tr -d ' ')
[ "$lines" -gt 1 ] || pc_fail "a 5000-byte line was not folded"
n=$(tr -cd x <o | wc -c | tr -d ' ')
[ "$n" = "5000" ] || pc_fail "folding lost or added characters: $n"
bad=$(sed '$d' o | grep -v '^x*\\$' | wc -l | tr -d ' ')
[ "$bad" = "0" ] || pc_fail "a folded line does not end with a backslash"
last=$(sed -n '$p' o)
case $last in *'$') ;; *) pc_fail "the last line does not end with a dollar sign: $last" ;; esac
pc_pass
