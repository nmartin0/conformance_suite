# trap - condition does nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action - is accepted and the condition keeps its trap.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	hyphen) return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
