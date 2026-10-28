# large and zero n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ngettext.003
# n of 0 and a large n select the plural.
. "$PC_CORE/pc.sh"
pc_begin XCU.ngettext.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
LC_ALL=C; export LC_ALL
mkdir tdd || pc_unresolved "setup: cannot make a directory"
TEXTDOMAINDIR=$PWD/tdd; export TEXTDOMAINDIR
unset TEXTDOMAIN LANGUAGE NLSPATH
pc_test
ngettext -d dom file files 1000000 >out 2>err </dev/null || pc_fail "large n: nonzero status"
printf '%s' files >exp
cmp out exp >/dev/null 2>&1 || pc_fail "n=1000000 wrote '$(cat out)'"
ngettext -d dom file files 0 >out 2>err </dev/null || pc_fail "n=0: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "n=0 wrote '$(cat out)'"
ngettext -d dom file files 10 >out 2>err </dev/null || pc_fail "n=10: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "n=10 wrote '$(cat out)'"
pc_pass
