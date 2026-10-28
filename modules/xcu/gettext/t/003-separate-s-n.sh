# -s with -n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gettext.003
# -s writes each msgid separated by a space and ends with a newline, -n
# omits that newline, and -E is the default with -s.
. "$PC_CORE/pc.sh"
pc_begin XCU.gettext.003
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
gettext -s -d dom one two three >out 2>err </dev/null || pc_fail "-s: nonzero status"
printf 'one two three\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-s wrote: $(od -c out | head -2)"
gettext -s -n -d dom one two >out 2>err </dev/null || pc_fail "-s -n: nonzero status"
printf 'one two' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-s -n wrote: $(od -c out | head -2)"
gettext -s -d dom 'a\tb' >out 2>err </dev/null || pc_fail "-s with an escape: nonzero status"
printf '%s\n' 'a\tb' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-s did not default to -E: $(od -c out | head -2)"
gettext -s -e -d dom 'a\tb' c >out 2>err </dev/null || pc_fail "-s -e: nonzero status"
printf 'a\tb c\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-s -e wrote: $(od -c out | head -2)"
pc_pass
