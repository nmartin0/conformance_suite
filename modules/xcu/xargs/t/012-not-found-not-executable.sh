# Exit statuses 127 and 126.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.012
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.012
LC_ALL=C; export LC_ALL
d=pcxargs012.$$
trap 'rm -rf "$d"' EXIT
mkdir "$d" || pc_fail "cannot make $d"
: >"$d/noexec" || pc_fail "cannot make a file"
chmod 644 "$d/noexec" || pc_fail "cannot chmod"
pc_test
echo x | xargs "$d/nosuchutility" >out 2>err
rc=$?
[ "$rc" -eq 127 ] || pc_fail "xargs of a missing utility exited with status $rc, expected 127"
echo x | xargs pcnosuchutility9z >out 2>err
rc=$?
[ "$rc" -eq 127 ] || pc_fail "xargs of a utility missing from PATH exited with status $rc, expected 127"
echo x | xargs "$d/noexec" >out 2>err
rc=$?
[ "$rc" -eq 126 ] || pc_fail "xargs of a file without execute permission exited with status $rc, expected 126"
pc_pass
