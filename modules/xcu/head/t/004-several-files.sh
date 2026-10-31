# With several file operands head writes a header line before each file's part, with no blank line before the first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.004
# MUTANT    head.header_marks
# MUTANT    head.header_nospace
# MUTANT    head.header_no_blank
# MUTANT    head.header_blank_first
# MUTANT    head.header_basename
# MUTANT    head.header_stderr
# MUTANT    head.header_joined
# MUTANT    head.header_blank_after
. "$PC_CORE/pc.sh"
pc_begin XCU.head.004
LC_ALL=POSIX
export LC_ALL
NL='
'
mkdir sub && printf '1\n2\n' >f1 && printf 'b1\nb2\n' >f2 && printf 'z1\n' >sub/x || pc_fail "cannot create input"
pc_test
got=$(head -n 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}${NL}==> f2 <==${NL}b1${NL}rc=0" ] || pc_fail "headers or separation between files are wrong"
got=$(head -c 2 f1 f2 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}${NL}==> f2 <==${NL}b1${NL}==> f1 <==${NL}1${NL}rc=0" ] || pc_fail "headers are wrong with three files and -c"
got=$(head -n 1 f1 sub/x 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}${NL}==> sub/x <==${NL}z1${NL}rc=0" ] || pc_fail "the header does not carry the pathname as given"
got=$(head -c 1 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "==> f1 <==${NL}1${NL}==> f2 <==${NL}brc=0" ] || pc_fail "the separator after output that ends mid-line is wrong"
pc_pass
