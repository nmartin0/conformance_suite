# length, index, substr, match, split, sub, gsub, sprintf, tolower and toupper return the specified values.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.012
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.012
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'print length("hello"), length("")')" = "5 0" ] || pc_fail "length of a string wrong"
[ "$(echo hello | awk '{ print length, length() }' 2>/dev/null)" = "5 5" ] || pc_fail "length of \$0 wrong"
[ "$(e 'a[1]; a[2]; print length(a)')" = "2" ] || pc_fail "length of an array wrong"
[ "$(e 'print index("foobar", "bar"), index("foo", "z"), index("foo", "o")')" = "4 0 2" ] || pc_fail "index wrong"
[ "$(e 'print substr("hello", 2, 3), substr("hello", 3), substr("hello", 4, 100), "[" substr("hello", 1, 0) "]"')" = "ell llo lo []" ] || pc_fail "substr wrong"
[ "$(e 's = "aaa"; n = sub(/a/, "b", s); print n, s')" = "1 baa" ] || pc_fail "sub wrong"
[ "$(e 's = "aaa"; n = gsub(/a/, "b", s); print n, s')" = "3 bbb" ] || pc_fail "gsub wrong"
[ "$(e 's = "abc"; gsub(/b/, "[&]", s); print s')" = "a[b]c" ] || pc_fail "& in the replacement wrong"
[ "$(e 's = "abc"; gsub(/b/, "\\&", s); print s')" = "a&c" ] || pc_fail "an escaped & in the replacement wrong"
[ "$(e 's = "abc"; n = gsub(/z/, "y", s); print n, s')" = "0 abc" ] || pc_fail "gsub with no match wrong"
[ "$(e 's = "a.b.c"; gsub("[.]", "-", s); print s')" = "a-b-c" ] || pc_fail "gsub with a string regular expression wrong"
[ "$(echo 'hello world' | awk '{ sub(/o/, "0"); print; print NF }' 2>/dev/null)" = "hell0 world${NL}2" ] || pc_fail "sub on \$0 wrong"
[ "$(echo 'a-b-c' | awk '{ gsub(/-/, " "); print NF }' 2>/dev/null)" = "3" ] || pc_fail "gsub on \$0 did not re-split the fields"
[ "$(echo 'a b' | awk '{ sub(/a/, "x", $1); print $0 }' 2>/dev/null)" = "x b" ] || pc_fail "sub on a field wrong"
[ "$(e 'print sprintf("%03d", 7), sprintf("%s-%s", "a", "b")')" = "007 a-b" ] || pc_fail "sprintf wrong"
[ "$(e 'print toupper("abC1"), tolower("ABc1")')" = "ABC1 abc1" ] || pc_fail "toupper or tolower wrong"
pc_pass
