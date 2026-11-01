# the shell exits after running a trap action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# After the action runs the shell terminates instead of going on.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- "$pcm_act${pcm_nl}exit" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
