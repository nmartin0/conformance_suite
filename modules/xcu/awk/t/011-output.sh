# print and printf write to standard output, files, appended files and pipes, and printf supports the common conversions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.011
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.011
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'old\n' >o2 && printf 'old\n' >o3 || pc_fail "cannot create files"
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'OFS="-"; ORS="!\n"; print "a", "b"')" = "a-b!" ] || pc_fail "print with OFS and ORS wrong"
[ "$(e 'print "x" > "o1"; print "y" > "o1"; close("o1"); while ((getline l < "o1") > 0) print "r:" l')" = "r:x${NL}r:y" ] || pc_fail "print > file did not keep appending while the file was open"
awk 'BEGIN { print "new" >> "o2" }' 2>/dev/null </dev/null
got=$(cat o2)
[ "$got" = "old${NL}new" ] || pc_fail ">> did not append"
awk 'BEGIN { print "new" > "o3" }' 2>/dev/null </dev/null
got=$(cat o3)
[ "$got" = "new" ] || pc_fail "> did not truncate an existing file"
[ "$(e 'print "b" | "sort"; print "a" | "sort"; close("sort"); print "done"')" = "a${NL}b${NL}done" ] || pc_fail "output to a pipe wrong"
[ "$(e 'printf "%d|%5d|%-5d|%05d\n", 42, 42, 42, 42')" = "42|   42|42   |00042" ] || pc_fail "printf %d forms wrong"
[ "$(e 'printf "%s|%10s|%-10s|%.2s\n", "abc", "abc", "abc", "abc"')" = "abc|       abc|abc       |ab" ] || pc_fail "printf %s forms wrong"
[ "$(e 'printf "%c%c\n", 65, "hello"')" = "Ah" ] || pc_fail "printf %c wrong"
[ "$(e 'printf "%x %X %o %u\n", 255, 255, 8, 7')" = "ff FF 10 7" ] || pc_fail "printf %x %X %o %u wrong"
[ "$(e 'printf "%5.2f|%e|%g\n", 3.14159, 1234.5, 0.0001')" = " 3.14|1.234500e+03|0.0001" ] || pc_fail "printf %f %e %g wrong"
[ "$(e 'printf "%*d|%.*f\n", 5, 42, 1, 2.5')" = "   42|2.5" ] || pc_fail "printf * width and precision wrong"
[ "$(e 'printf "100%%\n"')" = "100%" ] || pc_fail "printf %% wrong"
[ "$(e 'printf "a\tb\n"')" = "a$(printf '\t')b" ] || pc_fail "an escape in the format string wrong"
[ "$(e 'x = sprintf("%03d-%s", 7, "z"); print x')" = "007-z" ] || pc_fail "sprintf wrong"
pc_pass
