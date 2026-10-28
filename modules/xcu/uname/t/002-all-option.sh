# uname -a starts with uname -mnrsv.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.002
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.002
LC_ALL=C; export LC_ALL
nl='
'
pc_test
base=$(uname -mnrsv) || pc_fail "uname -mnrsv failed"
all=$(uname -a) || pc_fail "uname -a failed"
[ -n "$base" ] || pc_fail "uname -mnrsv wrote nothing"
case $all in
*"$nl"*) pc_fail "uname -a wrote more than one line" ;;
esac
if [ "$all" = "$base" ]; then
	pc_pass
fi
case $all in
"$base "*) pc_pass ;;
*) pc_fail "uname -a wrote '$all', which does not begin with '$base'" ;;
esac
