# a numeric first operand is taken as the action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The numeric first operand is read as an action for the conditions after it.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	num) pcm_a=$1; shift; command trap -- "$pcm_a " "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
