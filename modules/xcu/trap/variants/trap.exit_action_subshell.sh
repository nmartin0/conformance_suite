# a conforming shell that runs the EXIT action in a copy of the final environment
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the EXIT action must run in an environment identical to the one after the last command, which a forked copy is; nothing runs after it that could see its changes (XCU 2.15 trap, DESCRIPTION).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) command trap -- "(${pcm_nl}$pcm_act${pcm_nl})" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
