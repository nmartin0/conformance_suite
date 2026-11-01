# a second trap on a condition is not taken
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# When the condition already has an action, a later trap does not replace it.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set|null|hyphen)
		shift "$pcm_n"
		for pcm_c; do
			pcm_get "$pcm_c"
			case $pcm_val in A:*) continue ;; esac
			case $pcm_kind in
			set) pcm_put "$pcm_c" "A:$pcm_act" ;;
			null) pcm_put "$pcm_c" I ;;
			hyphen) pcm_put "$pcm_c" R ;;
			esac
			command trap -- "$pcm_act" "$pcm_c"
		done ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
