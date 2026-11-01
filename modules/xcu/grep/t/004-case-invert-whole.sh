# grep -i ignores case, -v selects the lines that do not match, and -x requires the whole line to match.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.004
# MUTANT    grep.i_ignored
# MUTANT    grep.v_ignored
# MUTANT    grep.x_ignored
# MUTANT    grep.x_start
# MUTANT    grep.x_end
# MUTANT    grep.x_alt_anchor
# MUTANT    grep.x_lost_with_i
# MUTANT    grep.i_lost_with_v
# MUTANT    grep.v_lost_with_x
# MUTANT    grep.v_null_dropped
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'Hello\nhello world\nHELLO\nbye\n' >in || pc_fail "cannot create input"
pc_test
got=$(grep -i hello in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "Hello${NL}hello world${NL}HELLO${NL}rc=0" ] || pc_fail "-i did not select the three hello lines"
got=$(grep -v hello in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "Hello${NL}HELLO${NL}bye${NL}rc=0" ] || pc_fail "-v did not select the lines without hello"
got=$(grep -x hello in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=1" ] || pc_fail "-x selected a line that is not exactly hello"
got=$(grep -x 'hello world' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hello world${NL}rc=0" ] || pc_fail "-x did not select a line matching entirely"
got=$(grep -ix 'HELLO' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "Hello${NL}HELLO${NL}rc=0" ] || pc_fail "-i with -x gave the wrong lines"
got=$(grep -v '' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=1" ] || pc_fail "-v with a null pattern selected a line"
pc_pass
