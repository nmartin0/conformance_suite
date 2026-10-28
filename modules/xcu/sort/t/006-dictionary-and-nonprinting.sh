# sort -d considers only blanks and alphanumerics, sort -i ignores nonprintable characters.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a-c\nab\nac\n' >in1 || pc_fail "cannot create input"
printf 'a\001c\nab\n' >in2 || pc_fail "cannot create input"
pc_test
got=$(sort -d in1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}a-c${NL}ac${NL}rc=0" ] || pc_fail "-d did not ignore the hyphen in the key"
got=$(sort -i in2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}a$(printf '\001')c${NL}rc=0" ] || pc_fail "-i did not ignore the nonprintable character in the key"
pc_pass
