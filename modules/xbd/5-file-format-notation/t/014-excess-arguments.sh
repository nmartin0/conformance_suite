# The awk printf statement ignores arguments left over once the format is used.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.5-file-format-notation.014
. "$PC_CORE/pc.sh"
pc_begin XBD.5-file-format-notation.014
LC_ALL=POSIX
export LC_ALL
pc_test
got=$(awk 'BEGIN { printf "%d|", 1, 2 }' 2>/dev/null </dev/null)
[ "$got" = "1|" ] || pc_fail "awk printf \"%d|\", 1, 2 wrote '$got', expected '1|'"
got=$(awk 'BEGIN { printf "%s-%s|", "a", "b", "c" }' 2>/dev/null </dev/null)
[ "$got" = "a-b|" ] || pc_fail "awk printf \"%s-%s|\", \"a\", \"b\", \"c\" wrote '$got', expected 'a-b|'"
got=$(awk 'BEGIN { printf "|", 5 }' 2>/dev/null </dev/null)
[ "$got" = "|" ] || pc_fail "awk printf \"|\", 5 wrote '$got', expected '|'"
pc_pass
