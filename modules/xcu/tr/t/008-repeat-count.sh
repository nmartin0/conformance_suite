# In string2, [x*n] stands for n copies of x, and [x*] for as many as are needed; a leading zero makes n octal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.008
# MUTANT    tr.rep_off
# MUTANT    tr.rep_one
# MUTANT    tr.fill_one
# MUTANT    tr.fill_to_end
# MUTANT    tr.rep_octal_dec
# MUTANT    tr.rep_zero_empty
# MUTANT    tr.rep_firstdigit
# MUTANT    tr.rep_x_escape_raw
# MUTANT    tr.no_final_newline
# VARIANT   tr.pad_last
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcd\n' >in1 || pc_fail "cannot create input"
printf 'abcdefghi\n' >in2 || pc_fail "cannot create input"
printf 'abcde\n' >in3 || pc_fail "cannot create input"
printf 'abcdefghijklm\n' >in4 || pc_fail "cannot create input"
printf 'ab, cd.ef\n' >in5 || pc_fail "cannot create input"
pc_test
got=$(tr abcd '[x*2]yz' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxyz${NL}rc=0" ] || pc_fail "tr abcd '[x*2]yz' gave the wrong result"
got=$(tr abcd '[x*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxxx${NL}rc=0" ] || pc_fail "tr abcd '[x*]' gave the wrong result"
got=$(tr abcd 'w[x*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "wxxx${NL}rc=0" ] || pc_fail "tr abcd 'w[x*]' gave the wrong result"
got=$(tr abcdefghi '[x*010]y' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxxxxxxxy${NL}rc=0" ] || pc_fail "tr with [x*010] did not repeat 8 times"
got=$(tr abcde 'a[x*]e' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "axxxe${NL}rc=0" ] || pc_fail "tr abcde 'a[x*]e': the fill did not stop before the rest of string2"
got=$(tr abcd 'w[x*0]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "wxxx${NL}rc=0" ] || pc_fail "tr abcd 'w[x*0]': a zero count did not fill"
got=$(tr abcdefghijklm '[x*12]y' <in4 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "xxxxxxxxxxxxy${NL}rc=0" ] || pc_fail "tr with [x*12] did not repeat 12 times"
got=$(tr -cs '[:alpha:]' '[\n*]' <in5 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}cd${NL}ef${NL}rc=0" ] || pc_fail "tr -cs '[:alpha:]' '[\\n*]' gave the wrong result"
pc_pass
