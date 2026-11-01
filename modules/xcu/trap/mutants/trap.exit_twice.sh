# the EXIT action runs twice when the shell exits
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action text of an EXIT trap is run two times in a row.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) command trap -- "$pcm_act${pcm_nl}$pcm_act" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
