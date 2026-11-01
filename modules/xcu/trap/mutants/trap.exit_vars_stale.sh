# the EXIT action sees the variable as it was when the trap was set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The variable pcx is put back to its value at installation before the EXIT action runs.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) pcm_v0=${pcx-}; command trap -- "pcx=\$pcm_v0${pcm_nl}$pcm_act" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
