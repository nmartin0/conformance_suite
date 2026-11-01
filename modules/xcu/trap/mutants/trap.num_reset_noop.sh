# a first operand that is a number does not reset
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The first operand being an unsigned decimal integer is not recognised and nothing is reset.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	num) return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
