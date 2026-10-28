# A cut list may be separated by blanks inside one argument.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.004
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -b "1 3 5-6" in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "acef${NL}rc=0" ] || pc_fail "-b '1 3 5-6' did not give acef"
got=$(cut -c "2 4" in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bd${NL}rc=0" ] || pc_fail "-c '2 4' did not give bd"
pc_pass
