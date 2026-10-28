# -e and -E
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gettext.002
# -e processes C escape sequences in the msgid and -E leaves them as they
# are.
. "$PC_CORE/pc.sh"
pc_begin XCU.gettext.002
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
gettext -e -d dom 'a\tb\n' >out 2>err </dev/null || pc_fail "-e: nonzero status"
printf 'a\tb\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-e did not process the escapes: $(od -c out | head -2)"
gettext -E -d dom 'a\tb\n' >out 2>err </dev/null || pc_fail "-E: nonzero status"
printf '%s' 'a\tb\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-E altered the escapes: $(od -c out | head -2)"
gettext -e -d dom 'x\\y\101\"z' >out 2>err </dev/null || pc_fail "-e (more escapes): nonzero status"
printf '%s' 'x\yA"z' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-e processed backslash, octal or quote escapes wrongly: $(od -c out | head -2)"
pc_pass
