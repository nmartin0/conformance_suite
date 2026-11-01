# trap -p names another condition in its line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# USR1 in the output of -p is written as USR2.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed 's/USR1/USR2/' "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
