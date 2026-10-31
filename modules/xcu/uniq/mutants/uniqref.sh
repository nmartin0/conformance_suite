# Shared by the uniq faults: a small reimplementation of uniq for the POSIX
# locale (a character is a byte) with one planted fault, chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_uniqref FAULT ARGS...: behaves correctly unless FAULT applies.
pc_uniqref_awk=${PC_MUTANT_FILE%/*}/uniqref.awk
pc_uniqref() (
	M=$1
	shift
	mode= nf=0 nc=0
	OPTIND=1
	while getopts cduf:s: o; do
		case $o in
		c | d | u) mode=$o ;;
		f) nf=$OPTARG ;;
		s) nc=$OPTARG ;;
		*) exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	in=${1:-} out=${2:-}
	if [ "$M" = no_stdin ] && [ $# -eq 0 ]; then
		echo "uniq: no input file" >&2
		exit 1
	fi
	[ "$M" = swap_operands ] && [ $# -eq 2 ] && { in=$2 out=$1; }
	[ "$M" = stdin_always ] && in=
	[ "$M" = dash_literal ] || [ "$in" != - ] || in=
	if [ -n "$in" ] && [ ! -r "$in" -o -d "$in" ]; then
		case $M in
		missing_no_diag) ;;
		missing_diag_stdout) echo "uniq: $in: cannot open" ;;
		*) echo "uniq: $in: cannot open" >&2 ;;
		esac
		[ "$M" = missing_status0 ] && exit 0
		exit 1
	fi
	run() {
		awk -v M="$M" -v mode="$mode" -v nf="$nf" -v nc="$nc" -f "$pc_uniqref_awk"
	}
	if [ -n "$in" ]; then exec <"$in"; fi
	if [ -n "$out" ]; then
		if [ "$M" = out_ignored ]; then
			run
			rc=$?
		elif [ "$M" = out_append ]; then
			run >>"$out" 2>/dev/null
			rc=$?
		else
			{ run >"$out"; } 2>/dev/null
			rc=$?
			if [ "$rc" -ne 0 ] || [ ! -e "$out" ]; then
				case $M in
				outerr_no_diag) ;;
				*) echo "uniq: $out: cannot write" >&2 ;;
				esac
				[ "$M" = outerr_status0 ] && exit 0
				exit 1
			fi
		fi
		[ "$M" = out_also_stdout ] && { run <"$out" 2>/dev/null; }
		[ "$M" = status1 ] && rc=1
		exit "${rc:-0}"
	fi
	run
	rc=$?
	[ "$M" = status1 ] && rc=1
	exit "$rc"
)
