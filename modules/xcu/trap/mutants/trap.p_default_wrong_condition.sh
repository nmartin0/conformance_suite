# trap -p of a default condition names another condition
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# For a condition in its default state the line written resets USR1 instead.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		shift
		for pcm_c; do
			command trap -p "$pcm_c" >"$pcm_dir/pc-mut.pl" 2>/dev/null
			if [ -s "$pcm_dir/pc-mut.pl" ]; then cat "$pcm_dir/pc-mut.pl"; else printf 'trap -- - USR1\n'; fi
		done ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
