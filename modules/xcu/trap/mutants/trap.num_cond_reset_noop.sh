# trap - with a signal number does not reset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Resetting a condition given as a number does nothing.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	hyphen)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in *[!0-9]*) command trap -- - "$pcm_c" ;; esac
		done ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
