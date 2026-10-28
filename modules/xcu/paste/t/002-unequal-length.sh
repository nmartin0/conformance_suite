# A file that ends early is treated as supplying empty lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.002
. "$PC_CORE/pc.sh"
pc_begin XCU.paste.002
LC_ALL=POSIX
export LC_ALL
NL='
'
T=$(printf '\t')
printf 'a1\na2\na3\n' >f1 && printf 'b1\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(paste f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a1${T}b1${NL}a2${T}${NL}a3${T}${NL}rc=0" ] || pc_fail "short last file was not padded with empty lines"
got=$(paste f2 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b1${T}a1${NL}${T}a2${NL}${T}a3${NL}rc=0" ] || pc_fail "short first file was not padded with empty lines"
pc_pass
