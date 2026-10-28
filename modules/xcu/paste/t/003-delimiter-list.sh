# paste -d takes its delimiters from the list in turn, starting again at the first for each line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.003
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\na2\n' >f1 && printf 'b1\nb2\n' >f2 && printf 'c1\nc2\n' >f3 && printf 'd1\nd2\n' >f4 || pc_fail "cannot create input"
pc_test
got=$(paste -d ',:' f1 f2 f3 f4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1,b1:c1,d1${NL}a2,b2:c2,d2${NL}rc=0" ] || pc_fail "-d ',:' with four files is wrong"
got=$(paste -d '-' f1 f2 f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1-b1-c1${NL}a2-b2-c2${NL}rc=0" ] || pc_fail "-d '-' with three files is wrong"
got=$(paste -d ',:' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1,b1${NL}a2,b2${NL}rc=0" ] || pc_fail "-d ',:' with two files is wrong"
pc_pass
