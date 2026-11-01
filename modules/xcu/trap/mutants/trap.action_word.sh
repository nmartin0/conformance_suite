# the action is run as one command name, not as shell text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action is treated as the name of a command, so a compound text fails.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; pcm_wa=$pcm_act; command trap -- '"$pcm_wa"' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
