# A line beginning with + replaces the newline of the previous line by characters that return to column 1; a leading + at the start of input acts as a space.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.004
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf ' ab\n+cd\n' >in || pc_fail "cannot create input"
pc_test
asa <in >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "status was not 0"
got=$(cat out; printf 'x')
case $got in
"ab"*"cd${NL}x") ;;
*) pc_fail "the output was not ab, something, and cd with a final newline" ;;
esac
mid=${got#ab}
mid=${mid%"cd${NL}x"}
[ -n "$mid" ] || pc_fail "nothing was written between ab and cd to return to column 1"
case $mid in
*"$NL"*) pc_fail "the newline of the previous line was not replaced" ;;
esac
got=$(printf '+x\n' | asa 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "x${NL}rc=0" ] || pc_fail "a + at the start of input was not treated as a space"
printf ' a\n+b\n+c\n' | asa >out3 2>/dev/null
got=$(cat out3; printf 'x')
case $got in
a*b*c"${NL}x") ;;
*) pc_fail "two overprinted lines were not joined into one line" ;;
esac
case ${got%x} in
*"$NL"*"$NL"*) pc_fail "more than one newline for three overprinted lines" ;;
esac
pc_pass
