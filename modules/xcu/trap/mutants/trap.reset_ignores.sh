# trap - condition makes the signal ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action - sets the condition to be ignored instead of the default.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	hyphen) shift "$pcm_n"; command trap -- '' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
