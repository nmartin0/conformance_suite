# With several file operands head writes a header line before each file's part, with no blank line before the first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.004
. "$PC_CORE/pc.sh"
pc_begin XCU.head.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >f1 && printf 'b1\nb2\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(head -n 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}${NL}==> f2 <==${NL}b1${NL}rc=0" ] || pc_fail "headers or separation between files are wrong"
got=$(head -c 2 f1 f2 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}${NL}==> f2 <==${NL}b1${NL}==> f1 <==${NL}1${NL}rc=0" ] || pc_fail "headers are wrong with three files and -c"
pc_pass
