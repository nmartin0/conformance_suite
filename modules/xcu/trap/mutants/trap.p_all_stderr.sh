# trap -p without operands writes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The full listing is written to standard error.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		if [ $# -eq 1 ]; then pcm_conform "$@" >&2; else pcm_conform "$@"; fi ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
