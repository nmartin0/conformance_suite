# hard links in the verbose listing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.017
# Archiving f1 and its hard link f1l and listing with -v ends the second
# line with " == f1".
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
ln f1 f1l || pc_unresolved "setup: cannot make a hard link"
pc_test
pax -w -x ustar f1 f1l >a 2>err </dev/null || pc_fail "pax -w: nonzero status"
pax -v <a >o 2>err || pc_fail "pax -v: nonzero status"
line=$(sed -n 2p o)
case $line in
*' f1l == f1') ;;
*) pc_fail "the hard link line is '$line', expected it to end with ' f1l == f1'" ;;
esac
pc_pass
