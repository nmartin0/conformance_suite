# -e and -E
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ngettext.002
# -e processes C escape sequences in msgid and msgid_plural; -E leaves them
# alone.
. "$PC_CORE/pc.sh"
pc_begin XCU.ngettext.002
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
ngettext -e -d dom 'a\tb' 'c\td' 2 >out 2>err </dev/null || pc_fail "-e plural: nonzero status"
printf 'c\td' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-e did not process the plural escapes: $(od -c out | head -2)"
ngettext -e -d dom 'a\tb' 'c\td' 1 >out 2>err </dev/null || pc_fail "-e singular: nonzero status"
printf 'a\tb' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-e did not process the singular escapes: $(od -c out | head -2)"
ngettext -E -d dom 'a\tb' 'c\td' 2 >out 2>err </dev/null || pc_fail "-E plural: nonzero status"
printf '%s' 'c\td' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-E altered the plural: $(od -c out | head -2)"
ngettext -E -d dom 'a\tb' 'c\td' 1 >out 2>err </dev/null || pc_fail "-E singular: nonzero status"
printf '%s' 'a\tb' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "-E altered the singular: $(od -c out | head -2)"
pc_pass
