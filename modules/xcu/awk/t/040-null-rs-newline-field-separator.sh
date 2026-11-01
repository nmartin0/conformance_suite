# With a null RS a newline is a field separator whatever FS is.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.040
# MUTANT    awk.null_rs_blank_line_regex
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.040
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(printf 'a:b\nc:d\n\ne:f\n' | awk 'BEGIN { RS = ""; FS = ":" } { print NF }' 2>/dev/null)
[ "$got" = "4${NL}2" ] || pc_fail "with RS null a newline was not a field separator under FS=:"
got=$(printf 'a,b\nc,d\n' | awk 'BEGIN { RS = ""; FS = "," } { print NF }' 2>/dev/null)
[ "$got" = "4" ] || pc_fail "with RS null a newline was not a field separator under FS=,"
got=$(printf 'a b\nc d\n' | awk 'BEGIN { RS = "" } { print NF }' 2>/dev/null)
[ "$got" = "4" ] || pc_fail "with RS null a newline was not a field separator under the default FS"
pc_pass
