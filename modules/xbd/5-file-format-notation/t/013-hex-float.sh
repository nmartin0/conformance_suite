# The a and A conversions.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.013
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.013
LC_ALL=POSIX
export LC_ALL
pc_test
# For zero the exponent is zero; the digits are not otherwise pinned down.
got=$(awk 'BEGIN { printf "%a", 0 }' 2>/dev/null </dev/null)
printf '%s\n' "$got" | grep -Eq '^0x0(\.0*)?p[+-]0$' ||
	pc_fail "awk printf \"%a\", 0 wrote '$got', expected 0x0 and a zero exponent in lowercase"
got=$(awk 'BEGIN { printf "%A", 0 }' 2>/dev/null </dev/null)
printf '%s\n' "$got" | grep -Eq '^0X0(\.0*)?P[+-]0$' ||
	pc_fail "awk printf \"%A\", 0 wrote '$got', expected 0X0 and a zero exponent in uppercase"
got=$(awk 'BEGIN { printf "%a", 255 }' 2>/dev/null </dev/null)
printf '%s\n' "$got" | grep -Eq '^0x[1-9a-f](\.[0-9a-f]+)?p[+-](0|[1-9][0-9]*)$' ||
	pc_fail "awk printf \"%a\", 255 wrote '$got', not 0x, a non-zero hexadecimal digit, optional fraction, p, a sign and a decimal exponent in lowercase"
got=$(awk 'BEGIN { printf "%A", 255 }' 2>/dev/null </dev/null)
printf '%s\n' "$got" | grep -Eq '^0X[1-9A-F](\.[0-9A-F]+)?P[+-](0|[1-9][0-9]*)$' ||
	pc_fail "awk printf \"%A\", 255 wrote '$got', not 0X, a non-zero hexadecimal digit, optional fraction, P, a sign and a decimal exponent in uppercase"
pc_pass
