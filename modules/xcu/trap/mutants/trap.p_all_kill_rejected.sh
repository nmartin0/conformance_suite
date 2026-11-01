# the full listing has a KILL line that reading back reports as an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The full listing includes KILL and STOP lines, and putting them back is reported as an error.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		if [ $# -eq 1 ]; then pcm_conform "$@"; printf 'trap -- - KILL\ntrap -- - STOP\n'; else pcm_conform "$@"; fi ;;
	hyphen)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in KILL|STOP) echo "trap: cannot trap $pcm_c" >&2; return 1 ;; esac
		done
		command trap -- - "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
