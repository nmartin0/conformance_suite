# command -v writes the absolute pathname of a utility.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.003
. "$PC_CORE/pc.sh"
pc_begin XCU.command.003
mkdir bin || pc_fail "cannot make a directory"
printf ': >ran\n' >bin/pccmd || pc_fail "cannot write a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
base=$PWD
PATH=$base/bin:$PATH
pc_test
out=$(command -v pccmd 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -v exited $rc"
case $out in
/*) ;;
*) pc_fail "command -v wrote '$out', not an absolute pathname" ;;
esac
[ "$out" -ef "$base/bin/pccmd" ] || pc_fail "command -v wrote '$out', which is not the utility $base/bin/pccmd"
[ ! -e ran ] || pc_fail "command -v ran the utility"
pc_pass
