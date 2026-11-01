# a conforming shell whose full listing includes KILL and STOP
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because it is unspecified whether the full trap -p listing includes the conditions for the two signals that cannot be caught, provided they are accepted when read back (XCU 2.15 trap, DESCRIPTION).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p)
		pcm_conform "$@"
		if [ $# -eq 1 ]; then printf 'trap -- - KILL\ntrap -- - STOP\n'; fi
		return 0 ;;
	hyphen)
		shift "$pcm_n"
		for pcm_c; do
			case $pcm_c in KILL|STOP) continue ;; esac
			command trap -- - "$pcm_c"
		done
		return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
