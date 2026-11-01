# a signal ignored on entry can be trapped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A signal that was ignored when the shell started is caught after all: the kill command runs its action.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		for pcm_c; do
			case " ${PCM_IGN-} " in *" $pcm_c "*) pcm_put "$pcm_c" "$pcm_act" ;; esac
		done
		command trap -- "$pcm_act" "$@" ;;
	null)
		shift "$pcm_n"
		for pcm_c; do PCM_IGN="${PCM_IGN-} $pcm_c"; done
		export PCM_IGN
		command trap -- '' "$@" ;;
	*) command trap "$@" ;;
	esac
}
pcm_kill() {
	if [ "${1-}" = -s ]; then
		pcm_get "${2-}"
		if [ -n "$pcm_val" ]; then eval "$pcm_val"; return 0; fi
	fi
	command kill "$@"
}
alias kill=pcm_kill
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
