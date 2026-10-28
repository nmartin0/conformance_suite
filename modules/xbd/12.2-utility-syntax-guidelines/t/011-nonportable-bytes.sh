# Operands and option-arguments may hold bytes outside the portable character set.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.011
#
# In the POSIX locale every byte value is a character, so the byte 0351 is
# usable in a file name and in a pattern.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.011
LC_ALL=POSIX
export LC_ALL
hi=$(printf '\351')
[ "${#hi}" -eq 1 ] || pc_unresolved "cannot form a single non-portable byte"
printf 'HI\n' >"x${hi}y" || pc_unresolved "cannot create a file whose name holds the byte 0351"
printf 'a%sb\nplain\n' "$hi" >g3 || pc_unresolved "cannot create g3"
pc_test
got=$(cat "x${hi}y" 2>/dev/null </dev/null)
[ "$got" = HI ] || pc_fail "cat of a file name holding the byte 0351 gave '$got', expected HI"
got=$(grep -e "$hi" g3 2>/dev/null </dev/null)
[ "$got" = "a${hi}b" ] || pc_fail "grep -e with an option-argument holding the byte 0351 gave '$got', expected the line holding it"
pc_pass
