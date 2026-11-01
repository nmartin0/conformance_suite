# Shared by the kill faults and variants: install the planted kill under
# the name given, both as a shell function (the shell's own kill) and, as a
# program first on PATH, for a direct exec (faults named exec_* only the
# program).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_killmut NAME DIR: DIR holds killbody.sh
pc_killmut() {
	PC_KM_FAULT=$1
	mkdir -p pc-mutbin
	{
		printf '#!/bin/sh\nPC_KM_FAULT=%s\n' "$1"
		cat "$2/killbody.sh"
		printf 'kill "$@"\n'
	} >pc-mutbin/kill
	chmod +x pc-mutbin/kill
	PATH=$PWD/pc-mutbin:$PATH
	case $1 in
	exec_*) ;;
	*) . "$2/killbody.sh" ;;
	esac
}
