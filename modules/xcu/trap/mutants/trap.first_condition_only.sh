# only the first condition operand gets the action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Of several conditions given to one trap command, only the first is set.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- "$pcm_act" "$1" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
