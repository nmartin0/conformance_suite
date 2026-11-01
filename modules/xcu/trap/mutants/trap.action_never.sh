# the action of trap is never run: the signal is ignored instead
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A set action is replaced by the empty action, so the signal is ignored.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- '' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
