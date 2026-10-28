# The s flag i matches the regular expression without regard to case.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.006
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
[ "$(echo 'Hello hello' | sed 's/HELLO/X/i' 2>/dev/null)" = "X hello" ] || pc_fail "flag i did not match case-insensitively"
[ "$(echo 'Hello hello' | sed 's/HELLO/X/ig' 2>/dev/null)" = "X X" ] || pc_fail "flags i and g together failed"
[ "$(echo 'Hello' | sed 's/HELLO/X/' 2>/dev/null)" = "Hello" ] || pc_fail "without i the match ignored case"
pc_pass
