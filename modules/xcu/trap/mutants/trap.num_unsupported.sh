# signal numbers are not accepted as conditions
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A numeric condition is refused with a warning and a non-zero status.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in *[!0-9]*) ;; *) echo "trap: $pcm_c: invalid signal" >&2; return 1 ;; esac
		done
		command trap -- "$pcm_act" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
