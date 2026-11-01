# the action runs in a new shell process
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The action text is given to a separate shell as its script.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; pcm_sa=$pcm_act; command trap -- '"$pcm_real" -c "$pcm_sa"' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
