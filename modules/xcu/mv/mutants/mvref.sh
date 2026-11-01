# Shared by the mv faults and variants: a small reimplementation of the mv -i
# question and of the many-operand form, so that a fault can change one thing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.

# pc_mvi FAULT SRC DST: mv -i SRC DST done by hand: the question goes to
# standard error, one line of standard input is the reply, y or Y first means yes.
pc_mvi() (
	M=$1 src=$2 dst=$3
	if [ -e "$dst" ]; then
		case $M in
		no_prompt) ;;
		prompt_no_name) printf 'mv: overwrite the file? ' >&2 ;;
		prompt_stdout) printf 'mv: overwrite %s? ' "$dst" ;;
		prompt_nl) printf 'Replace the existing file: %s\nAnswer yes or no\n' "$dst" >&2 ;;
		prompt_quoted) printf 'mv: replace "%s" (y/n) ' "$dst" >&2 ;;
		*) printf 'mv: overwrite %s? ' "$dst" >&2 ;;
		esac
		read -r pc_r
		if [ "$M" = neg_moves ]; then command mv "$src" "$dst"; exit 0; fi
		case $M in yes_word) case $pc_r in yes | YES) pc_r=y ;; *) pc_r=n ;; esac ;; esac
		case $pc_r in
		[yY]*)
			if [ "$M" = yes_ignored ]; then exit 0; fi
			command mv "$src" "$dst" || exit 1
			if [ "$M" = yes_status1 ]; then exit 1; fi
			;;
		*)
			if [ "$M" = neg_removes_dst ]; then rm -f "$dst"; fi
			if [ "$M" = neg_removes_src ]; then rm -f "$src"; fi
			if [ "$M" = neg_status1 ]; then exit 1; fi
			;;
		esac
		exit 0
	fi
	command mv "$src" "$dst"
)

# pc_mvopt MODE ARGS...: the options -f and -i then two operands; MODE says
# which option decides: last (as the text has it), first, anyi, anyf.
pc_mvopt() (
	mode=$1
	shift
	eff=
	while [ $# -gt 0 ]; do
		case $1 in
		-i | -f)
			o=${1#-}
			case $mode in
			last) eff=$o ;;
			first) [ -n "$eff" ] || eff=$o ;;
			anyi) [ "$eff" = i ] || eff=$o ;;
			anyf) [ "$eff" = f ] || eff=$o ;;
			esac
			shift
			;;
		*) break ;;
		esac
	done
	if [ "$eff" = i ] && [ $# -eq 2 ]; then pc_mvi std "$1" "$2"; else command mv "$@"; fi
)

# pc_mvmany FAULT SRC... DIR: the directory form done one source at a time.
pc_mvmany() (
	M=$1
	shift
	d=
	for a; do d=$a; done
	n=$#
	i=0
	rc=0
	skip=
	for s; do
		i=$((i + 1))
		[ "$i" -lt "$n" ] || break
		if [ -n "$skip" ]; then skip=; continue; fi
		if [ "$M" = nodiag ]; then command mv "$s" "$d" 2>/dev/null; else command mv "$s" "$d"; fi
		r=$?
		if [ "$r" -ne 0 ]; then
			[ "$M" != abort ] || exit 1
			[ "$M" != skip_next ] || skip=1
			rc=1
		fi
		[ "$M" != status_last ] || rc=$r
	done
	[ "$M" != status0 ] || exit 0
	exit "$rc"
)
