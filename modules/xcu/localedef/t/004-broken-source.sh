# localedef with a symbolic name that no charmap defines, in LC_NUMERIC.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.localedef.004
. "$PC_CORE/pc.sh"
pc_begin XCU.localedef.004
d=pcldef004.$$
trap 'rm -rf "$d"' EXIT
v=$(getconf POSIX2_LOCALEDEF 2>/dev/null) || pc_notinuse "getconf cannot report POSIX2_LOCALEDEF"
case $v in
[1-9]*) ;;
*) pc_notinuse "POSIX2_LOCALEDEF is '$v'" ;;
esac
mkdir "$d" || pc_fail "cannot make $d"
printf 'LC_NUMERIC\ndecimal_point "<pc-no-such-symbol-9z>"\nthousands_sep ""\ngrouping -1\nEND LC_NUMERIC\n' >"$d/src"
pc_test
localedef -i "$d/src" "$d/loc" >"$d/out" 2>"$d/err" </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "localedef using an undefined symbolic name in LC_NUMERIC exited with status 0"
[ -e "$d/loc" ] && pc_fail "a locale was created from a source with an error"
pc_pass
