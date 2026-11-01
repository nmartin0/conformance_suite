# full trap -p lines lack the --
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The words "trap --" at the start of each line of the full listing become "trap".
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		if [ $# -eq 1 ]; then pcm_conform "$@" | sed 's/^trap -- /trap /'; else pcm_conform "$@"; fi ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
