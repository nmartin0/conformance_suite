# paste -s turns each file into one line, in command-line order, restarting the delimiter list for every file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.005
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.005
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a1\na2\na3\n' >f1 && printf 'b1\nb2\n' >f2 && : >empty || pc_fail "cannot create input"
pc_test
got=$(paste -s f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${T}a2${T}a3${NL}b1${T}b2${NL}rc=0" ] || pc_fail "-s did not make one tab-joined line per file"
got=$(paste -s -d ',:' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1,a2:a3${NL}b1,b2${NL}rc=0" ] || pc_fail "-s -d ',:' did not restart the list for the second file"
got=$(paste -s empty f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}b1${T}b2${NL}rc=0" ] || pc_fail "-s did not give a lone newline for an empty file"
pc_pass
