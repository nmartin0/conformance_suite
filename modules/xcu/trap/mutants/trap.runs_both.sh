# the new action is added to the old one
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A later trap on a condition runs after the earlier action instead of replacing it.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		for pcm_c; do
			pcm_get "$pcm_c"
			case $pcm_val in
			A:*) pcm_new="${pcm_val#A:}${pcm_nl}$pcm_act" ;;
			*) pcm_new=$pcm_act ;;
			esac
			pcm_put "$pcm_c" "A:$pcm_new"
			command trap -- "$pcm_new" "$pcm_c"
		done ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
