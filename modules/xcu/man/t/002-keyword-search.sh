# man -k selects summary lines matching a keyword
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.man.002
# With -k each output line matches at least one of the keyword operands,
# ignoring case (tested when the summary database has any line for the
# keywords sort and copy; the wording of purpose entries is unspecified).
. "$PC_CORE/pc.sh"
pc_begin XCU.man.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
PAGER=cat; export PAGER
pc_test
man -k sort </dev/null >ks 2>err
rc=$?
[ -s ks ] || pc_unresolved "no summary line contains the keyword sort (purpose wording is unspecified)"
[ "$rc" -eq 0 ] || pc_fail "man -k sort: exit status $rc"
grep -Eiv 'sort' ks >bad
[ -s bad ] && pc_fail "a line does not match the keyword: $(sed -n 1p bad)"
man -k sort copy </dev/null >kc 2>err || pc_fail "man -k sort copy: nonzero status"
[ -s kc ] || pc_unresolved "no summary line matched either keyword"
grep -Eiv 'sort|copy' kc >bad
[ -s bad ] && pc_fail "a line matches neither keyword: $(sed -n 1p bad)"
pc_pass
