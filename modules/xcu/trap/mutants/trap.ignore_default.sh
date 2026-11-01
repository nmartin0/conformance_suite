# trap "" condition leaves the default action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The empty action is read as a reset: the signal keeps its default action.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	null) shift "$pcm_n"; command trap -- - "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
