# trap -p writes only for the first operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Of several operands only the first is written.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p) shift; command trap -p "${1-}" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
