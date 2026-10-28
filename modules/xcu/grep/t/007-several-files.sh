# With more than one file, grep prefixes each line with the file name and a colon, numbering lines from 1 in each file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.007
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\na\n' >f1 && printf 'b\na\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(grep a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:a${NL}f1:a${NL}f2:a${NL}rc=0" ] || pc_fail "lines were not prefixed with file names"
got=$(grep -n a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:1:a${NL}f1:3:a${NL}f2:2:a${NL}rc=0" ] || pc_fail "-n did not restart at 1 for each file"
got=$(grep -c a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:2${NL}f2:1${NL}rc=0" ] || pc_fail "-c did not give f1:2 and f2:1"
got=$(grep a f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}a${NL}rc=0" ] || pc_fail "a single file operand produced a prefix"
pc_pass
