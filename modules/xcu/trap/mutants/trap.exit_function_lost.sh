# the EXIT action cannot call a function defined after the trap
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The function pcf is removed before an EXIT action runs.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) command trap -- "unset -f pcf${pcm_nl}$pcm_act" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
