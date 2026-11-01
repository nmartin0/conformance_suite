# a trap on EXIT is never set
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The condition EXIT is accepted and dropped; 0 still works.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; for pcm_c; do case $pcm_c in EXIT) ;; *) command trap -- "$pcm_act" "$pcm_c" ;; esac; done ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
