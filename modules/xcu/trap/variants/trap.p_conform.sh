# a conforming shell whose trap -p also writes conditions in the default state
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text asks trap -p without operands to list all conditions and gives no form for a default one beyond reading back; this system writes trap -- - COND for them (XCU 2.15 trap, DESCRIPTION and OPTIONS).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	p) pcm_conform "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
