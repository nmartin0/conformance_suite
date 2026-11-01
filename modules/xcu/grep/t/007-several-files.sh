# With more than one file, grep prefixes each line with the file name and a colon, numbering lines from 1 in each file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.007
# MUTANT    grep.prefix_missing
# MUTANT    grep.prefix_dash
# MUTANT    grep.prefix_space
# MUTANT    grep.prefix_basename
# MUTANT    grep.prefix_single
# MUTANT    grep.n_before_file
# MUTANT    grep.n_no_reset
# MUTANT    grep.stdin_extra
# MUTANT    grep.c_zero_silent
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\na\n' >f1 && printf 'b\na\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(grep a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:a${NL}f1:a${NL}f2:a${NL}rc=0" ] || pc_fail "lines were not prefixed with file names"
got=$(grep -n a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:1:a${NL}f1:3:a${NL}f2:2:a${NL}rc=0" ] || pc_fail "-n did not restart at 1 for each file"
got=$(grep -c a f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:2${NL}f2:1${NL}rc=0" ] || pc_fail "-c did not give f1:2 and f2:1"
got=$(grep a f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}a${NL}rc=0" ] || pc_fail "a single file operand produced a prefix"
got=$(grep -c zzz f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "f1:0${NL}f2:0${NL}rc=1" ] || pc_fail "-c did not give a zero count for each file"
mkdir d && printf 'a\n' >d/g1 && printf 'a\n' >d/g2 || pc_fail "cannot create input"
got=$(grep a d/g1 d/g2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "d/g1:a${NL}d/g2:a${NL}rc=0" ] || pc_fail "the prefix was not the file operand as given"
printf 'other\na\n' >sin || pc_fail "cannot create input"
got=$(grep a f2 <sin 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}rc=0" ] || pc_fail "the standard input was searched although a file operand was given"
pc_pass
