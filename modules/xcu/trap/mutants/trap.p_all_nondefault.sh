# trap -p without operands lists only conditions that are set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The default-state lines of the full listing are left out.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		if [ $# -eq 1 ]; then pcm_conform "$@" | grep -v '^trap -- - '; else pcm_conform "$@"; fi; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
