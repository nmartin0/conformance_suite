# pax and symbolic links
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.018
# By default a symbolic link is archived itself (its -v line starts with l);
# with -L or -H a link to a regular file named on the command line is
# archived as that file under the link's name (the -v line starts with -).
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.018
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\n' >f1
ln -s f1 sl || pc_unresolved "setup: cannot make a symbolic link"
pc_test
pax -w -x ustar sl >a 2>err </dev/null || pc_fail "pax -w sl: nonzero status"
pax -v <a >o 2>err || pc_fail "pax -v: nonzero status"
c=$(cut -c1 o)
[ "$c" = l ] || pc_fail "the default archived a symbolic link as type '$c'"
pax -w -x ustar -L sl >b 2>err </dev/null || pc_fail "pax -w -L: nonzero status"
pax -v <b >o 2>err || pc_fail "pax -v: nonzero status"
c=$(cut -c1 o)
[ "$c" = - ] || pc_fail "-L archived the link as type '$c'"
case $(cat o) in *' sl') ;; *) pc_fail "-L did not use the link's name: $(cat o)" ;; esac
pax -w -x ustar -H sl >c 2>err </dev/null || pc_fail "pax -w -H: nonzero status"
pax -v <c >o 2>err || pc_fail "pax -v: nonzero status"
c=$(cut -c1 o)
[ "$c" = - ] || pc_fail "-H archived the link as type '$c'"
pc_pass
