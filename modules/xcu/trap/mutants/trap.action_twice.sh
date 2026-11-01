# the action runs twice each time the signal arrives
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action text is run two times in a row.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- "$pcm_act${pcm_nl}$pcm_act" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
