# text domain from operand, -d and TEXTDOMAIN
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gettext.001
# With a text domain given as an operand, as -d, or in TEXTDOMAIN, and no
# catalog, the msgid is written to standard output unchanged and without a
# newline, with status 0.
. "$PC_CORE/pc.sh"
pc_begin XCU.gettext.001
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
for form in 1 2 3 4; do
	case $form in
	1) gettext -d dom 'hello world' >out 2>err </dev/null ;;
	2) gettext dom 'hello world' >out 2>err </dev/null ;;
	3) TEXTDOMAIN=dom gettext 'hello world' >out 2>err </dev/null ;;
	4) TEXTDOMAIN=other gettext -d dom 'hello world' >out 2>err </dev/null ;;
	esac
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "form $form: exit status $rc"
	printf '%s' 'hello world' >exp
	cmp out exp >/dev/null 2>&1 || pc_fail "form $form wrote '$(cat out)' (len $(wc -c <out)), expected hello world with no newline"
done
gettext -d dom other 'hello world' >out 2>err </dev/null || pc_fail "operand and -d together: nonzero status"
printf '%s' 'hello world' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "an operand textdomain with -d wrote '$(cat out)'"
pc_pass
