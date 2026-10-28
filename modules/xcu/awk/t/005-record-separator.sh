# RS as one character ends records at that character; a null RS separates records at blank lines and makes newline a field separator.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.005
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.005
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
got=$(printf 'a;b;c;' | awk 'BEGIN { RS = ";" } { print NR ":" $0 }' 2>/dev/null)
[ "$got" = "1:a${NL}2:b${NL}3:c" ] || pc_fail "RS=; gave the wrong records"
got=$(printf 'a b\nc\n\n\n\nd e\n' | awk 'BEGIN { RS = "" } { print NR ":" NF ":" $1 ":" $3 }' 2>/dev/null)
[ "$got" = "1:3:a:c${NL}2:2:d:" ] || pc_fail "RS null did not give paragraph records"
got=$(printf 'a:b\nc:d\n\ne:f\n' | awk 'BEGIN { RS = ""; FS = ":" } { print NF }' 2>/dev/null)
[ "$got" = "4${NL}2" ] || pc_fail "with RS null a newline was not a field separator under FS=:"
got=$(printf '\n\na\n\nb\n' | awk 'BEGIN { RS = "" } END { print NR }' 2>/dev/null)
[ "$got" = "2" ] || pc_fail "leading blank lines produced an empty record with RS null"
got=$(printf 'a\nb\n\n\n' | awk 'BEGIN { RS = "" } END { print NR }' 2>/dev/null)
[ "$got" = "1" ] || pc_fail "trailing blank lines produced an empty record with RS null"
got=$(printf 'x\ny\n' | awk '{ print NR ":" $0 }' 2>/dev/null)
[ "$got" = "1:x${NL}2:y" ] || pc_fail "the default RS did not separate lines"
got=$(printf 'a1b1c1' | awk -v RS=1 '{ print $0 }' 2>/dev/null)
[ "$got" = "a${NL}b${NL}c" ] || pc_fail "-v RS=1 gave the wrong records"
pc_pass
