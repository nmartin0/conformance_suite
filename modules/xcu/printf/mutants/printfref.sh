# Shared by the printf faults and variants: a reimplementation of the
# utility with one planted fault (printfref.awk, run by printfrun.sh).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_pfref_both NAME   the shell's printf and the one found on PATH both
# pc_pfref_sh NAME     only the shell's printf (a function)
# pc_pfref_exec NAME   only the one found on PATH (a direct exec)
pc_pfdir=${PC_MUTANT_FILE%/*}
case $pc_pfdir in
/*) ;;
*) pc_pfdir=$PWD/$pc_pfdir ;;
esac
case $pc_pfdir in
*/variants) pc_pfdir=${pc_pfdir%/variants}/mutants ;;
esac
pc_pfref_sh() {
	pc_pfname=$1
	eval 'printf() { PC_PF_FAULT='"$pc_pfname"' sh "'"$pc_pfdir"'/printfrun.sh" "$@"; }'
}
pc_pfref_exec() {
	mkdir -p pc-mutbin
	{
		echo '#!/bin/sh'
		echo "PC_PF_FAULT=$1 exec sh \"$pc_pfdir/printfrun.sh\" \"\$@\""
	} >pc-mutbin/printf
	chmod +x pc-mutbin/printf
	PATH=$PWD/pc-mutbin:$PATH
}
pc_pfref_both() {
	pc_pfref_sh "$1"
	pc_pfref_exec "$1"
}
