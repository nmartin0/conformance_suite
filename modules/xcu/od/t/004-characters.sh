# od -t c writes printable bytes as themselves, the C escapes for the named controls, a single backslash for backslash, \0 for NUL and three octal digits for other nonprintable bytes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.004
# MUTANT    od.c_n_oct
# MUTANT    od.c_t_oct
# MUTANT    od.c_r_oct
# MUTANT    od.c_b_oct
# MUTANT    od.c_f_oct
# MUTANT    od.c_v_oct
# MUTANT    od.c_a_oct
# MUTANT    od.c_nul_oct
# MUTANT    od.c_nul_bsbs
# MUTANT    od.c_bs_double
# MUTANT    od.c_bs_octal
# MUTANT    od.c_esc_e
# MUTANT    od.c_space_oct
# MUTANT    od.c_del_raw
# MUTANT    od.c_printable_oct
# MUTANT    od.c_ctl_short
# MUTANT    od.c_ctl_hex
# MUTANT    od.c_ctl_long
# VARIANT   od.gap3
# VARIANT   od.an_empty
# VARIANT   od.trail_blank
. "$PC_CORE/pc.sh"
pc_begin XCU.od.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\n\t\\\000\001\r\b\f\v\007' >f1 || pc_fail "cannot create input"
printf 'a b' >f2 || pc_fail "cannot create input"
printf '\033\177\037z' >f3 || pc_fail "cannot create input"
pc_test
got=$(od -A n -v -t c f1 2>/dev/null </dev/null)
set -- $got
[ $# -eq 11 ] || pc_fail "eleven bytes did not give eleven fields"
[ "$1" = a ] || pc_fail "a printable byte was not written as itself"
[ "$2" = '\n' ] || pc_fail "newline is not \\n"
[ "$3" = '\t' ] || pc_fail "tab is not \\t"
[ "$4" = '\' ] || pc_fail "backslash is not a single backslash"
[ "$5" = '\0' ] || pc_fail "NUL is not \\0"
[ "$6" = 001 ] || pc_fail "byte 1 is not 001"
[ "$7" = '\r' ] || pc_fail "carriage return is not \\r"
[ "$8" = '\b' ] || pc_fail "backspace is not \\b"
[ "$9" = '\f' ] || pc_fail "form feed is not \\f"
[ "${10}" = '\v' ] || pc_fail "vertical tab is not \\v"
[ "${11}" = '\a' ] || pc_fail "alert is not \\a"
# a space is printable and is written as itself, so it leaves no field
got=$(od -A n -v -t c f2 2>/dev/null </dev/null)
set -- $got
[ "$*" = "a b" ] || pc_fail "a space was not written as itself"
got=$(od -A n -v -t c f3 2>/dev/null </dev/null)
set -- $got
[ "$*" = "033 177 037 z" ] || pc_fail "other nonprintable bytes are not three octal digits"
pc_pass
