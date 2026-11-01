# in a command substitution the listing is empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# When trap runs in a subshell, nothing is listed.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list)
		"$pcm_real" -c 'echo $PPID' >"$pcm_dir/pc-mut.pp"
		read pcm_pp <"$pcm_dir/pc-mut.pp"
		if [ "$pcm_pp" != "$$" ]; then return 0; fi
		command trap "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
