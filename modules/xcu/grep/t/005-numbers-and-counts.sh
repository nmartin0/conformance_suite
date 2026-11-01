# grep -n prefixes line numbers, and -c writes only the count of selected lines.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.005
# MUTANT    grep.n_ignored
# MUTANT    grep.n_dash
# MUTANT    grep.n_zero_based
# MUTANT    grep.n_selected_count
# MUTANT    grep.c_ignored
# MUTANT    grep.c_off_by_one
# MUTANT    grep.c_zero_silent
# MUTANT    grep.c_v_ignored
# MUTANT    grep.c_n_lines
# MUTANT    grep.c_to_stderr
# MUTANT    grep.c_nonl
# MUTANT    grep.c_zero_status0
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\na\nc\na\n' >in || pc_fail "cannot create input"
pc_test
got=$(grep -n a in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1:a${NL}3:a${NL}5:a${NL}rc=0" ] || pc_fail "-n did not give 1:a 3:a 5:a"
got=$(grep -c a in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}rc=0" ] || pc_fail "-c did not give 3"
got=$(grep -c -v a in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}rc=0" ] || pc_fail "-c -v did not give 2"
got=$(grep -c zzz in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "0${NL}rc=1" ] || pc_fail "-c with no match did not give 0 and status 1"
got=$(grep -c -n a in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "3${NL}rc=0" ] || pc_fail "-c with -n did not give only the count"
pc_pass
