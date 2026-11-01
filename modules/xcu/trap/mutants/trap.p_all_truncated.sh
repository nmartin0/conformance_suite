# trap -p without operands stops after a few conditions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Only the first three lines of the full listing are written.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		if [ $# -eq 1 ]; then pcm_conform "$@" | sed -n 1,3p; else pcm_conform "$@"; fi ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
