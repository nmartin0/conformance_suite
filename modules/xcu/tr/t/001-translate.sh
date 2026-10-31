# tr string1 string2 replaces each character of string1 by the character in the same position of string2, ranges included.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.001
# MUTANT    tr.map_off
# MUTANT    tr.map_first_only
# MUTANT    tr.swap_operands
# MUTANT    tr.range_literal
# MUTANT    tr.range_excl
# MUTANT    tr.range_start_excl
# MUTANT    tr.drops_unmatched
# MUTANT    tr.no_final_newline
# MUTANT    tr.extra_newline
# MUTANT    tr.status1
# MUTANT    tr.to_stderr
# MUTANT    tr.stderr_noise
# MUTANT    tr.large_truncate
# MUTANT    tr.nul_stops
# MUTANT    tr.nul_dropped
# VARIANT   tr.pad_last
# VARIANT   tr.dup_last
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aabbcc cab\n' >in || pc_fail "cannot create input"
printf 'aaaaaaaaa\n' >big || pc_fail "cannot create input"
printf 'bbbbbbbbb\n' >bigexp || pc_fail "cannot create input"
i=0
while [ "$i" -lt 14 ]; do
	cat big big >big2 && mv big2 big && cat bigexp bigexp >bigexp2 && mv bigexp2 bigexp ||
		pc_fail "cannot create the large input"
	i=$((i + 1))
done
pc_test
tr abc xyz <in >out 2>err
rc=$?
[ -f out ] || pc_fail "tr abc xyz left no output"
got=$(cat out; printf 'rc=%s' "$rc")
[ "$got" = "xxyyzz zxy${NL}rc=0" ] || pc_fail "tr abc xyz gave the wrong result"
[ ! -s err ] || pc_fail "tr wrote to standard error although it succeeded"
got=$(tr a-c A-C <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "AABBCC CAB${NL}rc=0" ] || pc_fail "tr a-c A-C gave the wrong result"
got=$(tr a-b 'xy' <in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxyycc cxy${NL}rc=0" ] || pc_fail "tr a-b xy gave the wrong result"
# characters that are not in string1 are copied, including a null byte
got=$(printf 'a\000a\n' | tr a b 2>/dev/null | od -An -v -tx1)
set -- $got
[ "$*" = "62 00 62 0a" ] || pc_fail "a null byte was not copied through"
# a large input is translated entirely
tr a b <big >bigout 2>/dev/null
rc=$?
[ -f bigout ] || pc_fail "tr left no output for a large input"
[ "$rc" -eq 0 ] || pc_fail "tr exited $rc on a large input"
cmp -s bigout bigexp || pc_fail "a large input was not translated entirely"
pc_pass
