# trap - resets only the first condition
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Of several conditions, only the first is reset.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	hyphen) shift "$pcm_n"; command trap -- - "$1" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
