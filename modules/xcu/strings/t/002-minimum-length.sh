# strings -n number sets the shortest run written.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strings.002
. "$PC_CORE/pc.sh"
pc_begin XCU.strings.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\000defgh\000ij\000klmn\nxyz\n' >in || pc_fail "cannot create input"
pc_test
list() {
	strings -a "$@" in 2>/dev/null </dev/null | while IFS= read -r l; do printf '%s,' "$l"; done
}
got=$(list -n 3)
case $got in
*abc,defgh,klmn,xyz,*) ;;
*) pc_fail "-n 3 did not write abc, defgh, klmn, xyz" ;;
esac
case $got in
*ij,*) pc_fail "-n 3 wrote a string of two characters" ;;
esac
got=$(list -n 2)
case $got in
*abc,defgh,ij,klmn,xyz,*) ;;
*) pc_fail "-n 2 did not write all five strings" ;;
esac
got=$(list -n 5)
case $got in
*defgh,*) ;;
*) pc_fail "-n 5 did not write defgh" ;;
esac
case $got in
*klmn,* | *abc,* | *ij,* | *xyz,*) pc_fail "-n 5 wrote a string shorter than five characters" ;;
esac
pc_pass
