# localedef -i with a missing file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.localedef.003
. "$PC_CORE/pc.sh"
pc_begin XCU.localedef.003
d=pcldef003.$$
trap 'rm -rf "$d"' EXIT
v=$(getconf POSIX2_LOCALEDEF 2>/dev/null) || pc_notinuse "getconf cannot report POSIX2_LOCALEDEF"
case $v in
[1-9]*) ;;
*) pc_notinuse "POSIX2_LOCALEDEF is '$v'" ;;
esac
mkdir "$d" || pc_fail "cannot make $d"
pc_test
localedef -i "$d/nosuchsource" "$d/loc" >"$d/out" 2>"$d/err" </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "localedef with a missing source file exited with status 0"
[ -e "$d/loc" ] && pc_fail "a locale was created although the source file was missing"
pc_pass
