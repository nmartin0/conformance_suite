# Shared by the cut faults: a small reimplementation of cut for the POSIX
# locale (a character is a byte) with one planted fault, chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_cutref FAULT SCOPE ARGS...: SCOPE is b, c, f or any; the fault is only
# active when the option letter given on the command line is SCOPE (any:
# always). Otherwise it behaves correctly.
pc_cutref_awk=${PC_MUTANT_FILE%/*}/cutref.awk
pc_cutref() (
	M=$1 scope=$2
	shift 2
	mode= list= delim=$(printf '\t') sflag= nflag= dgiven=
	OPTIND=1
	while getopts b:c:d:f:ns o; do
		case $o in
		b | c | f) mode=$o list=$OPTARG ;;
		d) delim=$OPTARG dgiven=1 ;;
		s) sflag=1 ;;
		n) nflag=1 ;;
		*) exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	[ "$scope" = any ] || [ "$scope" = "$mode" ] || M=none
	[ "$M" = delim_ignored ] && delim=$(printf '\t')
	[ "$M" = default_space ] && [ -z "$dgiven" ] && delim=' '
	if [ "$M" = no_operand_error ] && [ $# -eq 0 ]; then
		echo "cut: no file operand" >&2
		exit 1
	fi
	[ $# -eq 0 ] && set -- -
	if [ "$M" = reverse_files ]; then
		i=0
		for f; do
			i=$((i + 1))
			eval "v$i=\$f"
		done
		set --
		while [ "$i" -ge 1 ]; do
			eval 'set -- "$@" "$v'$i'"'
			i=$((i - 1))
		done
	fi
	rc=0
	for f; do
		if [ "$f" = - ] && [ "$M" != dash_literal ] && [ "$M" != dash_skipped ]; then
			awk -v M="$M" -v mode="$mode" -v list="$list" -v delim="$delim" \
				-v sflag="$sflag" -v nflag="$nflag" -f "$pc_cutref_awk" || rc=1
		elif [ "$f" = - ] && [ "$M" = dash_skipped ]; then
			continue
		elif [ -r "$f" ] && [ ! -d "$f" ]; then
			awk -v M="$M" -v mode="$mode" -v list="$list" -v delim="$delim" \
				-v sflag="$sflag" -v nflag="$nflag" -f "$pc_cutref_awk" <"$f" || rc=1
		else
			case $M in
			missing_no_diag) ;;
			missing_diag_stdout) echo "cut: $f: cannot open" ;;
			*) echo "cut: $f: cannot open" >&2 ;;
			esac
			case $M in
			missing_status0) ;;
			missing_abort) exit 1 ;;
			*) rc=1 ;;
			esac
		fi
		[ "$M" = only_first_file ] && break
	done
	exit "$rc"
)
