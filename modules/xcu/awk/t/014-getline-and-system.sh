# The getline forms read the next record, a record into a variable, from a file or from a command, return 1, 0 or -1, and system returns a command's status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.014
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.014
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\nc\n' >in3 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(awk 'NR == 1 { getline; print "got " $0 " NR=" NR }' in3 2>/dev/null </dev/null)
[ "$got" = "got b NR=2" ] || pc_fail "plain getline did not read the next record into \$0 and NR"
got=$(awk 'NR == 1 { getline x; print x, $0, NR }' in3 2>/dev/null </dev/null)
[ "$got" = "b a 2" ] || pc_fail "getline var changed \$0 or did not set NR"
got=$(awk 'NR == 1 { getline; getline; print NR, $0 }' in3 2>/dev/null </dev/null)
[ "$got" = "3 c" ] || pc_fail "two getlines did not reach the third record"
got=$(printf '1\n2\n3\n' | awk '{ r = getline; print r, $0 }' 2>/dev/null)
[ "$got" = "1 2${NL}0 3" ] || pc_fail "getline did not return 1 and then 0 at end of input"
got=$(awk 'BEGIN { while ((getline l < "in3") > 0) n++; print n, NR }' 2>/dev/null </dev/null)
[ "$got" = "3 0" ] || pc_fail "getline < file wrong or changed NR"
got=$(awk 'BEGIN { r = (getline l < "nosuchfile"); print r }' 2>/dev/null </dev/null)
[ "$got" = "-1" ] || pc_fail "getline from a missing file did not return -1"
got=$(awk 'BEGIN { f = "in3"; while ((getline l < f) > 0) ; r = (getline l < f); print r; close(f); r2 = (getline l < f); print r2, l }' 2>/dev/null </dev/null)
[ "$got" = "0${NL}1 a" ] || pc_fail "getline did not return 0 at end of file or did not reopen after close"
got=$(awk 'BEGIN { "echo hi" | getline x; print x; r = ("echo hi" | getline y); print r, "[" y "]" }' 2>/dev/null </dev/null)
[ "$got" = "hi${NL}0 []" ] || pc_fail "cmd | getline var wrong"
got=$(awk 'BEGIN { "echo a b" | getline; print NF, $2 }' 2>/dev/null </dev/null)
[ "$got" = "2 b" ] || pc_fail "cmd | getline did not set \$0 and NF"
got=$(awk 'BEGIN { "echo x" | getline; print close("echo x") }' 2>/dev/null </dev/null)
[ "$got" = "0" ] || pc_fail "close of a pipe did not return 0"
got=$(awk 'BEGIN { r = system("exit 3"); print r }' 2>/dev/null </dev/null)
[ "$got" = "3" ] || pc_fail "system did not return the command's status"
got=$(awk 'BEGIN { r = system("true"); print r }' 2>/dev/null </dev/null)
[ "$got" = "0" ] || pc_fail "system of a successful command did not return 0"
got=$(awk 'BEGIN { system("echo from-system"); print "after" }' 2>/dev/null </dev/null)
[ "$got" = "from-system${NL}after" ] || pc_fail "system output was not written before later output"
pc_pass
