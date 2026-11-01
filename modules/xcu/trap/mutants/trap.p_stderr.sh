# trap -p writes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The output of -p goes to standard error.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		cat "$pcm_dir/pc-mut.l1" >&2; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
