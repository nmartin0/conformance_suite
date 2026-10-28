# pattern space and hold space size
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.017
# The pattern space and the hold space can each hold at least 8192 bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.017
LC_ALL=POSIX
export LC_ALL
NL='
'
mkx() { s=x; while [ "${#s}" -lt "$1" ]; do s=$s$s; done; printf "%.${1}s" "$s"; }
mkx 8192 >big || pc_fail "cannot create input"
printf '\n' >>big
pc_test
n=$(sed 's/$/y/' big 2>/dev/null </dev/null | wc -c | tr -d ' ')
[ "$n" = "8194" ] || pc_fail "an 8192-byte pattern space lost data: $n bytes written"
n=$(sed -n 'h;s/x/z/;x;p' big 2>/dev/null </dev/null | wc -c | tr -d ' ')
[ "$n" = "8193" ] || pc_fail "an 8192-byte hold space lost data: $n bytes written"
n=$(sed -n 'h;x;g;p' big 2>/dev/null </dev/null | tr -d '\n' | wc -c | tr -d ' ')
[ "$n" = "8192" ] || pc_fail "copying an 8192-byte space changed it: $n bytes"
pc_pass
