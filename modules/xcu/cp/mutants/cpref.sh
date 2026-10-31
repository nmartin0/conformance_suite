# Shared by the cp faults and variants: operand analysis and a small
# reimplementation of the cp -i question, so that a fault can change one thing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_cp_parse ARGS...: sets pc_opts (the leading option words), pc_nops (the
# number of operands), pc_first, pc_last, pc_lastdir (set when the last
# operand is an existing directory), pc_R (set with -R) and pc_dest (where the
# copy of the first operand lands, for the two-operand and directory forms).
pc_cp_parse() {
	pc_opts=
	while [ $# -gt 0 ]; do
		case $1 in
		-?*) pc_opts="$pc_opts $1"; shift ;;
		*) break ;;
		esac
	done
	pc_nops=$#
	pc_first=${1-}
	pc_last=
	for pc_last; do :; done
	pc_lastdir=
	if [ -d "$pc_last" ]; then pc_lastdir=1; fi
	pc_R=
	case $pc_opts in *R*) pc_R=1 ;; esac
	pc_dest=$pc_last
	if [ -n "$pc_lastdir" ]; then pc_dest=${pc_last%/}/${pc_first##*/}; fi
	pc_simple=
	if [ "$pc_nops" -eq 2 ] && [ -z "$pc_lastdir" ] && [ -z "$pc_R" ]; then pc_simple=1; fi
	return 0
}

# pc_cpi FAULT SRC DST: cp -i SRC DST done by hand: the question goes to
# standard error, one line of standard input is the reply, y or Y first means yes.
pc_cpi() (
	M=$1 src=$2 dst=$3
	if [ -e "$dst" ]; then
		case $M in
		no_prompt) ;;
		prompt_no_name) printf 'cp: overwrite the file? ' >&2 ;;
		prompt_stdout) printf 'cp: overwrite %s? ' "$dst" ;;
		*) printf 'cp: overwrite %s? ' "$dst" >&2 ;;
		esac
		read -r pc_r
		if [ "$M" = neg_overwrites ]; then command cp "$src" "$dst"; exit 0; fi
		case $pc_r in
		[yY]*)
			if [ "$M" = yes_ignored ]; then exit 0; fi
			command cp "$src" "$dst" || exit 1
			if [ "$M" = yes_status1 ]; then exit 1; fi
			;;
		*)
			if [ "$M" = neg_removes ]; then rm -f "$dst"; fi
			if [ "$M" = neg_status1 ]; then exit 1; fi
			;;
		esac
		exit 0
	fi
	command cp "$src" "$dst"
)
