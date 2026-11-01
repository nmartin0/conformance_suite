# trap -p with operands writes every condition
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The operands of -p are dropped, so all set conditions are written.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p) command trap -p ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
