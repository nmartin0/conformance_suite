# sed selects lines by number, by $ for the last line, by /RE/ or \cREc, and between two addresses inclusively, counting lines across files.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >f1 && printf '4\n5\n/6\n' >f2 || pc_fail "cannot create input"
cat f1 f2 >all || pc_fail "cannot create input"
pc_test
run_case() {
	sed -n "$@" 2>/dev/null </dev/null | while IFS= read -r l; do printf '%s,' "$l"; done
}
[ "$(run_case 2p f1)" = "2," ] || pc_fail "a line number address failed"
[ "$(run_case '$p' f1 f2)" = "/6," ] || pc_fail "\$ was not the last line of the last file"
[ "$(run_case 4p f1 f2)" = "4," ] || pc_fail "line numbers were not cumulative across files"
[ "$(run_case '/[35]/p' all)" = "3,5," ] || pc_fail "/RE/ address failed"
[ "$(run_case '\,3,p' all)" = "3," ] || pc_fail "\\cREc address failed"
[ "$(run_case '\|^/6|p' all)" = "/6," ] || pc_fail "\\cREc with | failed"
[ "$(run_case '\x/6x p' all)" = "/6," ] || pc_fail "\\cREc with a blank before the command failed"
[ "$(run_case '2,4p' all)" = "2,3,4," ] || pc_fail "numeric range is not inclusive"
[ "$(run_case '/2/,/4/p' all)" = "2,3,4," ] || pc_fail "RE range is not inclusive"
[ "$(run_case '3,1p' all)" = "3," ] || pc_fail "range with second number <= first did not select one line"
[ "$(run_case '2,/[0-9]/p' all)" = "2,3," ] || pc_fail "the second RE address was not searched for from the line after the first"
[ "$(run_case '/[135]/,/[135]/p' all)" = "1,2,3,5,/6," ] || pc_fail "ranges were not restarted after ending"
[ "$(run_case '2,$p' all)" = "2,3,4,5,/6," ] || pc_fail "range ending at \$ failed"
[ "$(run_case '/\//p' all)" = "/6," ] || pc_fail "an escaped slash inside /RE/ failed"
pc_pass
