# Changes made in a subshell (variables, unset, working directory, mask,
# options, functions, traps, descriptors) do not affect the shell that
# started it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.005
mkdir pc.sub || pc_unresolved "mkdir"
PCV=out
PCK=keep
d0=$(pwd -P)
m0=$(umask)
{ : >&5; } 2>/dev/null && pc_unresolved "descriptor 5 is already open"
pc_test
(
	PCV=in
	PCN=new
	unset PCK
	cd pc.sub
	umask 077
	set -f
	pcsf() { :; }
	trap "echo t" USR1
	exec 5>pc.n5
	export PCX9=1
)
[ "$PCV" = out ] || pc_fail "variable changed to '$PCV'"
[ -z "${PCN+set}" ] || pc_fail "a variable set in the subshell is visible"
[ "$PCK" = keep ] || pc_fail "unset in the subshell removed the variable"
[ "$(pwd -P)" = "$d0" ] || pc_fail "working directory changed"
[ "$(umask)" = "$m0" ] || pc_fail "mask changed"
case $- in
*f*) pc_fail "option -f set in the subshell leaked" ;;
esac
(pcsf) 2>/dev/null
[ "$?" -eq 127 ] || pc_fail "a function defined in the subshell is visible"
t=$(trap)
case $t in
*USR1*) pc_fail "a trap set in the subshell is listed" ;;
esac
{ : >&5; } 2>/dev/null && pc_fail "descriptor opened by exec in the subshell is open"
r=$(${PC_SH:-sh} -c 'printf %s "${PCX9-u}"')
[ "$r" = u ] || pc_fail "export in the subshell leaked"
pc_pass
