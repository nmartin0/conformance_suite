# USR1 and USR2 are exchanged
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A trap on USR1 is put on USR2 and the other way round.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in USR1) pcm_c=USR2 ;; USR2) pcm_c=USR1 ;; esac
			set -- "$@" "$pcm_c"
			shift
		done
		command trap -- "$pcm_act" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
