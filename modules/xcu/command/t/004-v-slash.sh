# command -v with a slash in the name writes an absolute pathname.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.004
. "$PC_CORE/pc.sh"
pc_begin XCU.command.004
mkdir bin || pc_fail "cannot make a directory"
: >bin/pccmd || pc_fail "cannot write a file"
chmod +x bin/pccmd || pc_fail "cannot make the file executable"
base=$PWD
pc_test
out=$(command -v ./bin/pccmd 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v exited $rc"
case $out in
/*) ;;
*) pc_fail "command -v wrote '$out', not an absolute pathname" ;;
esac
[ "$out" -ef "$base/bin/pccmd" ] || pc_fail "command -v wrote '$out', which is not $base/bin/pccmd"
pc_pass
