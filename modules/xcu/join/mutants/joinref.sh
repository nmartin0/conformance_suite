# Shared by the join faults: a small reimplementation of join for the POSIX
# locale (a character is a byte) with one planted fault, chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_joinref FAULT ARGS...: behaves correctly unless FAULT applies.
pc_joinref_awk=${PC_MUTANT_FILE%/*}/joinref.awk
pc_joinref() (
	M=$1
	shift
	if [ "$M" = adjacent_rejected ]; then
		for pc_ja; do
			case $pc_ja in
			-[av][12])
				echo "join: invalid option: $pc_ja" >&2
				exit 1
				;;
			esac
		done
	fi
	a1=0 a2=0 v1=0 v2=0 e= eset=0 o= tset=0 sep= k1=1 k2=1
	OPTIND=1
	while getopts a:v:e:o:t:1:2: opt; do
		case $opt in
		a)
			[ "$M" = a_last_only ] && { a1=0 a2=0; }
			case $OPTARG in 1) a1=1 ;; 2) a2=1 ;; *) exit 1 ;; esac
			;;
		v)
			[ "$M" = v_last_only ] && { v1=0 v2=0; }
			case $OPTARG in 1) v1=1 ;; 2) v2=1 ;; *) exit 1 ;; esac
			;;
		e) e=$OPTARG eset=1 ;;
		o) o=$OPTARG ;;
		t) tset=1 sep=$OPTARG ;;
		1) k1=$OPTARG ;;
		2) k2=$OPTARG ;;
		*) echo "join: invalid option" >&2; exit 1 ;;
		esac
	done
	shift $((OPTIND - 1))
	if [ $# -ne 2 ]; then
		echo "join: two file operands are required" >&2
		exit 1
	fi
	[ "$M" = f1_ignored ] && k1=1
	[ "$M" = f2_ignored ] && k2=1
	if [ "$M" = swap_opts ]; then
		pc_t=$k1 k1=$k2 k2=$pc_t
	fi
	f1=$1 f2=$2
	if [ "$M" = stdin_swapped ] && { [ "$f1" = - ] || [ "$f2" = - ]; }; then
		f1=$2 f2=$1
	fi
	if [ "$M" = stdin_always_read ] && [ "$f1" != - ] && [ "$f2" != - ]; then
		cat >/dev/null
	fi
	bad=0
	for pc_side in 1 2; do
		eval "pc_f=\$f$pc_side"
		if [ "$pc_f" = - ]; then
			case $M in
			dash_literal) ;;
			dash_f1_literal) [ "$pc_side" = 1 ] || continue ;;
			dash_f2_literal) [ "$pc_side" = 2 ] || continue ;;
			dash_f2_ignored) if [ "$pc_side" = 2 ]; then eval "f$pc_side=/dev/null"; fi; continue ;;
			*) eval "f$pc_side=/dev/stdin"; continue ;;
			esac
		fi
		if [ ! -r "$pc_f" ] || [ -d "$pc_f" ]; then
			if [ "$M" = missing_a_ok ] && [ "$a1$a2$v1$v2" != 0000 ]; then
				eval "f$pc_side=/dev/null"
				continue
			fi
			case $M in
			missing_no_diag) ;;
			missing_diag_stdout) echo "join: $pc_f: cannot open" ;;
			*) echo "join: $pc_f: cannot open" >&2 ;;
			esac
			if [ "$M" = missing_status0 ] ||
				{ [ "$M" = missing1_status0 ] && [ "$pc_side" = 1 ]; } ||
				{ [ "$M" = missing2_status0 ] && [ "$pc_side" = 2 ]; }; then
				exit 0
			fi
			exit 1
		fi
	done
	if [ "$M" = to_stderr ]; then
		exec >&2
	fi
	awk -v M="$M" -v F1="$f1" -v F2="$f2" -v K1="$k1" -v K2="$k2" \
		-v A1="$a1" -v A2="$a2" -v V1="$v1" -v V2="$v2" \
		-v E="$e" -v ESET="$eset" -v OL="$o" -v TSET="$tset" -v SEP="$sep" \
		-f "$pc_joinref_awk"
	rc=$?
	[ "$M" = status1 ] && exit 1
	exit $rc
)
