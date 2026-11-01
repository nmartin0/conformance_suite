# trap without operands also lists a condition in its default state
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A line for TERM, which is in the default state, is added to the listing.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list) command trap "$@" || return $?; printf 'trap -- - TERM\n' ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
