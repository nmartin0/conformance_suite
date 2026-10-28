# default singular or plural from n
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ngettext.001
# With no catalog the msgid is written for n equal to 1 and the msgid_plural
# otherwise, whether the domain comes from an operand, -d or TEXTDOMAIN; n
# is read as a decimal integer so 001 means 1.
. "$PC_CORE/pc.sh"
pc_begin XCU.ngettext.001
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
for form in 1 2 3; do
	for n in 1 001; do
		case $form in
		1) ngettext -d dom file files $n >out 2>err </dev/null ;;
		2) ngettext dom file files $n >out 2>err </dev/null ;;
		3) TEXTDOMAIN=dom ngettext file files $n >out 2>err </dev/null ;;
		esac
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "form $form, n=$n: exit status $rc"
		printf '%s' file >exp
		cmp out exp >/dev/null 2>&1 || pc_fail "form $form, n=$n wrote '$(cat out)', expected file"
	done
	for n in 0 2 5; do
		case $form in
		1) ngettext -d dom file files $n >out 2>err </dev/null ;;
		2) ngettext dom file files $n >out 2>err </dev/null ;;
		3) TEXTDOMAIN=dom ngettext file files $n >out 2>err </dev/null ;;
		esac
		rc=$?
		[ "$rc" -eq 0 ] || pc_fail "form $form, n=$n: exit status $rc"
		printf '%s' files >exp
		cmp out exp >/dev/null 2>&1 || pc_fail "form $form, n=$n wrote '$(cat out)', expected files"
	done
done
pc_pass
