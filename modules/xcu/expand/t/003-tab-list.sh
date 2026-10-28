# expand -t with a list puts tabs at the listed columns, and a tab past the last one becomes a single space.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expand.003
. "$PC_CORE/pc.sh"
pc_begin XCU.expand.003
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a%sb%sc%sd\n' "$T" "$T" "$T" >in || pc_fail "cannot create input"
pc_test
got=$(expand -t 4,10 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a   b     c d${NL}rc=0" ] || pc_fail "-t 4,10 gave the wrong result"
got=$(expand -t '4 10' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a   b     c d${NL}rc=0" ] || pc_fail "-t '4 10' gave the wrong result"
pc_pass
