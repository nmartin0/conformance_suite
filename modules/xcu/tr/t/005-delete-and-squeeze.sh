# tr -d -s deletes the characters of string1 and then squeezes runs of characters of string2.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.005
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'axx  b\n' >in || pc_fail "cannot create input"
pc_test
got=$(tr -ds x ' ' <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a b${NL}rc=0" ] || pc_fail "tr -ds x ' ' gave the wrong result"
pc_pass
