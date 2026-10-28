# -k ignores the case of the keyword
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.man.003
# A keyword given in upper case finds lines for the same purpose entries as
# the lower case keyword.
. "$PC_CORE/pc.sh"
pc_begin XCU.man.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
PAGER=cat; export PAGER
pc_test
man -k sort </dev/null >lo 2>err
rc=$?
[ -s lo ] || pc_unresolved "no summary line contains the keyword sort (purpose wording is unspecified)"
[ "$rc" -eq 0 ] || pc_fail "man -k sort: exit status $rc"
man -k SORT </dev/null >up 2>err || pc_fail "man -k SORT: nonzero status"
[ -s up ] || pc_fail "man -k SORT found nothing although man -k sort did"
grep -Eiv 'sort' up >bad
[ -s bad ] && pc_fail "a line does not match the upper case keyword"
cmp lo up >/dev/null 2>&1 || pc_fail "upper case keyword gave different lines"
pc_pass
