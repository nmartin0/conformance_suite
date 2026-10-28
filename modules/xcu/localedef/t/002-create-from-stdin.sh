# localedef reads the source from standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.localedef.002
. "$PC_CORE/pc.sh"
pc_begin XCU.localedef.002
d=pcldef002.$$
trap 'rm -rf "$d"' EXIT
v=$(getconf POSIX2_LOCALEDEF 2>/dev/null) || pc_notinuse "getconf cannot report POSIX2_LOCALEDEF"
case $v in
[1-9]*) ;;
*) pc_notinuse "POSIX2_LOCALEDEF is '$v'" ;;
esac
mkdir "$d" || pc_fail "cannot make $d"
for c in LC_CTYPE LC_COLLATE LC_MONETARY LC_NUMERIC LC_TIME LC_MESSAGES; do
	printf '%s\ncopy "POSIX"\nEND %s\n' "$c" "$c"
done >"$d/src"
pc_test
localedef -c "$d/loc" >"$d/out" 2>"$d/err" <"$d/src"
rc=$?
case $rc in
0 | 1) ;;
2) pc_unresolved "localedef reported exit status 2 (limits or unsupported code set)" ;;
3) pc_fail "localedef exited with status 3, creating locales is not supported although POSIX2_LOCALEDEF is $v" ;;
*) pc_fail "localedef exited with status $rc" ;;
esac
[ -e "$d/loc" ] || pc_fail "no locale exists at the pathname given as name"
pc_pass
