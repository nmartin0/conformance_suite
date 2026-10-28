# command -V describes how a name is read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.008
. "$PC_CORE/pc.sh"
pc_begin XCU.command.008
mkdir bin || pc_fail "cannot make a directory"
printf ': >ran\n' >bin/pccmd || pc_fail "cannot write a script"
chmod +x bin/pccmd || pc_fail "cannot make the script executable"
PATH=$PWD/bin:$PATH
pcfn() { :; }
alias pcal8='echo six'
pc_test
out=$(command -V pccmd 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -V for a utility exited $rc"
case $out in
*/bin/pccmd*) ;;
*) pc_fail "command -V for a utility wrote '$out', without its pathname" ;;
esac
[ ! -e ran ] || pc_fail "command -V ran the utility"
out=$(command -V pcfn 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -V for a function exited $rc"
case $out in
*[Ff]unction*) ;;
*) pc_fail "command -V for a function wrote '$out', which does not say function" ;;
esac
out=$(command -V pcal8 2>/dev/null)
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command -V for an alias exited $rc"
case $out in
*[Aa]lias*) ;;
*) pc_fail "command -V for an alias wrote '$out', which does not say alias" ;;
esac
case $out in
*'echo six'*) ;;
*) pc_fail "command -V for an alias wrote '$out', without the value" ;;
esac
pc_pass
