# In an s replacement, & stands for the match, \n for the nth group, a backslash removes special meaning, an escaped newline splits the line, and another delimiter may be used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.004
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
t() {
	printf '%s\n' "$1" | sed "$2" 2>/dev/null
}
[ "$(t abc 's/b/[&]/')" = "a[b]c" ] || pc_fail "& was not replaced by the match"
[ "$(t abc 's/b/[\&]/')" = "a[&]c" ] || pc_fail "\\& was not literal"
[ "$(t abc 's/\(a\)\(b\)/\2\1/')" = "bac" ] || pc_fail "back-references \\1 \\2 failed"
[ "$(t abc 's/a\(x\)*b/<\1>/')" = "<>c" ] || pc_fail "a back-reference with no match was not empty"
[ "$(t 'a\b' 's/\\/\\\\/')" = 'a\\b' ] || pc_fail "an escaped backslash in the replacement failed"
[ "$(t 'a/b' 's|/|:|')" = "a:b" ] || pc_fail "an alternative delimiter failed"
[ "$(t 'x,y' 's,x\,y,Z,')" = "Z" ] || pc_fail "an escaped delimiter in the RE failed"
[ "$(t 'a-b' 's,-,\,,')" = "a,b" ] || pc_fail "an escaped delimiter in the replacement failed"
[ "$(t 'a%b' 's%[%]%+%')" = "a+b" ] || pc_fail "a delimiter inside a bracket expression ended the RE"
got=$(printf 'a,b\n' | sed 's/,/\
/' 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "an escaped newline in the replacement did not split the line"
[ "$(t 'a+b' 's/+/&&/')" = "a++b" ] || pc_fail "doubled & failed"
pc_pass
