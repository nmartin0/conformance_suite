# trap - on an invalid condition is accepted silently
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Only the reset form accepts an unknown name without a word.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	hyphen) command trap "$@" 2>/dev/null; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
