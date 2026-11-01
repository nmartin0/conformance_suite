# a numeric first operand resets only that condition
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# With a numeric first operand only that one condition is reset, not the others after it.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	num) command trap -- - "$1" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
