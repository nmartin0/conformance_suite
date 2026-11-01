# trap -p output loses the quotes of the action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Single quotes are removed from the output of -p.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed "s/'//g" "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
