# Shared by the tr faults and variants: a small reimplementation of tr for
# the POSIX locale (a character is a byte) with one planted fault or
# variant, chosen by name. The work is done in trref.awk.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_trref NAME ARGS...: runs as tr would with ARGS. NAME is a fault or a
# variant of the reference (see trref.awk); none behaves correctly.
pc_trref_dir=${PC_MUTANT_FILE%/*}
case $pc_trref_dir in
*/variants) pc_trref_dir=${pc_trref_dir%/variants}/mutants ;;
esac
pc_trref_awk=$pc_trref_dir/trref.awk
pc_trref() (
	M=$1
	shift
	CF= DF= SF=
	OPTIND=1
	while getopts cCds o; do
		case $o in
		c) CF=c ;;
		C) CF=C ;;
		d) DF=1 ;;
		s) SF=1 ;;
		*) echo "tr: bad option" >&2; exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	NOPS=$# S1=${1-} S2=${2-}
	export M CF DF SF NOPS S1 S2
	case $M in
	c_status1) [ "$CF" = c ] && st=1 ;;
	C_status1) [ "$CF" = C ] && st=1 ;;
	d_status1) [ -n "$DF" ] && st=1 ;;
	s_status1) [ -n "$SF" ] && st=1 ;;
	status1) st=1 ;;
	to_stderr) exec 1>&2 ;;
	stderr_noise) echo "tr: processing" >&2 ;;
	esac
	od -An -v -tu1 | LC_ALL=C awk -f "$pc_trref_awk"
	[ -z "${st-}" ] || exit 1
	exit 0
)
