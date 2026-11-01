# csplit -f sets the file name prefix and -n the number of digits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.005
# MUTANT    csplit.f_ignored
# MUTANT    csplit.n_ignored
# MUTANT    csplit.n_plus1
# MUTANT    csplit.n_minus1
# MUTANT    csplit.n_hex
# MUTANT    csplit.n_spaces
# MUTANT    csplit.f_basename
# MUTANT    csplit.default_digits_3
# MUTANT    csplit.default_digits_1
# MUTANT    csplit.file_start1
# MUTANT    csplit.names_reversed
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
csplit -s -f part -n 3 in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -f part -n 3 gave status $rc"
got=$(cat part000; printf x)
[ "$got" = "1${NL}2${NL}3${NL}4${NL}x" ] || pc_fail "part000 is wrong"
got=$(cat part001; printf x)
[ "$got" = "5${NL}6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "part001 is wrong"
[ -e xx00 ] && pc_fail "a file with the default prefix was created"
[ -e part00 ] && pc_fail "a file with the default two digits was created"
csplit -s -n 4 in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -n 4 gave status $rc"
[ -s xx0000 ] && [ -s xx0001 ] || pc_fail "-n 4 did not give xx0000 and xx0001"
rm -f xx0000 xx0001 part000 part001
# the default number of digits is two when only a prefix is given
csplit -s -f pre in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -f pre gave status $rc"
[ -s pre00 ] && [ -s pre01 ] || pc_fail "-f pre did not give pre00 and pre01"
[ -e pre000 ] && pc_fail "-f pre made a file with three digits"
rm -f pre00 pre01
# one digit
csplit -s -f one -n 1 in10 4 8 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -n 1 gave status $rc"
[ -s one0 ] && [ -s one1 ] && [ -s one2 ] || pc_fail "-n 1 did not give one0, one1 and one2"
[ -e one00 ] && pc_fail "-n 1 made a file with two digits"
rm -f one0 one1 one2
# file numbers are decimal: twelve files
i=0
while [ "$i" -lt 24 ]; do
	i=$((i + 1))
	printf 'n%s\n' "$i"
done >in24
csplit -s -f dec in24 2 '{10}' >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit 2 {10} on 24 lines gave status $rc"
[ -s dec10 ] && [ -s dec11 ] || pc_fail "the files dec10 and dec11 were not made"
[ -e dec0a ] && pc_fail "a file number was written in hexadecimal"
[ -e dec12 ] && pc_fail "a thirteenth file was made"
rm -f dec[0-9][0-9] dec0a dec0b
# a prefix that names a directory
mkdir sub || pc_fail "cannot make a directory"
csplit -s -f sub/q in10 5 >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "csplit -f sub/q gave status $rc"
[ -s sub/q00 ] && [ -s sub/q01 ] || pc_fail "-f sub/q did not give sub/q00 and sub/q01"
[ -e q00 ] && pc_fail "the directory part of the prefix was dropped"
pc_pass
