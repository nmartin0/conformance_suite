# sort -m merges files that are already ordered.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.011
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nc\ne\n' >f1 && printf 'b\nd\nf\n' >f2 && printf 'a\nb\n' >f3 || pc_fail "cannot create input"
pc_test
got=$(sort -m f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}c${NL}d${NL}e${NL}f${NL}rc=0" ] || pc_fail "-m did not merge two ordered files"
got=$(sort -m f1 f3 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}a${NL}b${NL}c${NL}e${NL}rc=0" ] || pc_fail "-m lost or reordered lines"
pc_pass
