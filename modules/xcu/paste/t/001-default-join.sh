# paste joins corresponding lines of its files with tabs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.001
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.001
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a1\na2\n' >f1 && printf 'b1\nb2\n' >f2 && printf 'c1\nc2\n' >f3 || pc_fail "cannot create input"
pc_test
got=$(paste f1 f2 f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${T}b1${T}c1${NL}a2${T}b2${T}c2${NL}rc=0" ] || pc_fail "three files were not joined line by line with tabs"
got=$(paste f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${NL}a2${NL}rc=0" ] || pc_fail "one file was not copied unchanged"
pc_pass
