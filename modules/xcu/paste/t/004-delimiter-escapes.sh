# In a paste -d list, backslash-t, backslash-n, double backslash and backslash-zero stand for tab, newline, backslash and the empty string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.004
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.004
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a1\na2\n' >f1 && printf 'b1\nb2\n' >f2 && printf 'c1\nc2\n' >f3 || pc_fail "cannot create input"
pc_test
got=$(paste -d '\t' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${T}b1${NL}a2${T}b2${NL}rc=0" ] || pc_fail "-d '\\t' did not give a tab"
got=$(paste -d '\\' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = 'a1\b1'"${NL}"'a2\b2'"${NL}rc=0" ] || pc_fail "-d '\\\\' did not give a backslash"
got=$(paste -d '\0' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1b1${NL}a2b2${NL}rc=0" ] || pc_fail "-d '\\0' did not give the empty string"
got=$(paste -d '\n' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${NL}b1${NL}a2${NL}b2${NL}rc=0" ] || pc_fail "-d '\\n' did not give a newline"
got=$(paste -d '\0,' f1 f2 f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1b1,c1${NL}a2b2,c2${NL}rc=0" ] || pc_fail "-d '\\0,' with three files is wrong"
pc_pass
