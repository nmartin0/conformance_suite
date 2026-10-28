# uuencode begin line carries the input file mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.004
# For an input file with permission bits rw-r----- the begin line holds that
# mode, in octal (640) or in symbolic notation that chmod turns into those
# bits, and the decode pathname.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'Cat' >f || pc_unresolved "setup: cannot write input"
chmod 640 f || pc_unresolved "setup: chmod failed"
: >chk || pc_unresolved "setup: cannot create check file"
pc_test
uuencode f dest.bin >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
l1=$(sed -n 1p out)
case $l1 in
begin\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
mode=${l1#begin }
mode=${mode% dest.bin}
case $mode in
'' | *' '*) pc_fail "no single mode field in the begin line: $l1" ;;
*[!0-7]*)
	chmod 000 chk || pc_unresolved "cannot clear the mode of the check file"
	chmod "$mode" chk || pc_fail "mode '$mode' is not accepted by chmod"
	perms=$(ls -l chk | cut -c1-10)
	[ "$perms" = '-rw-r-----' ] || pc_fail "symbolic mode '$mode' gives $perms, expected -rw-r-----"
	;;
*)
	m=${mode#"${mode%%[!0]*}"}
	[ "$m" = 640 ] || pc_fail "octal mode is '$mode', expected 640"
	;;
esac
pc_pass
