# the number 15 is taken for USR1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# On trap the condition number 15 is set on USR1.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in 15) pcm_c=USR1 ;; esac
			set -- "$@" "$pcm_c"
			shift
		done
		command trap -- "$pcm_act" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
