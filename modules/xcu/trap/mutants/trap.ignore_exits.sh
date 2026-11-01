# an ignored signal ends the shell quietly
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The empty action is made to end the shell when the signal arrives.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	null) shift "$pcm_n"; command trap -- 'exit 0' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
