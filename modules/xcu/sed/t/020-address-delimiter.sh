# escaping the delimiter in a context address
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.020
# In an address \cREc an escaped c is a literal c that does not end the RE
# (also for the slash delimiter).
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.020
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcxdef\nabc/def\na,b\nother\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed -n '\xabc\xdefxp' in 2>/dev/null </dev/null)
[ "$got" = "abcxdef" ] || pc_fail "\\xabc\\xdefx : $got"
got=$(sed -n '/abc\/def/p' in 2>/dev/null </dev/null)
[ "$got" = "abc/def" ] || pc_fail "/abc\\/def/ : $got"
got=$(sed -n '\,a\,b,p' in 2>/dev/null </dev/null)
[ "$got" = "a,b" ] || pc_fail "\\,a\\,b, : $got"
pc_pass
