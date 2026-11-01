# a conforming shell that reports an error for a signal ignored on entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text says no error need be reported when trying to trap or reset a signal ignored on entry, so one may be; the shell keeps the signal ignored and goes on (XCU 2.15 trap, DESCRIPTION).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set|hyphen)
		shift "$pcm_n"
		for pcm_c; do
			case " ${PCM_IGN-} " in
			*" $pcm_c "*) echo "trap: $pcm_c was ignored on entry" >&2; return 1 ;;
			esac
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
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
