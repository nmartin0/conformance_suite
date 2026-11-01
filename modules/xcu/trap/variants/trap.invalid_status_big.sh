# a conforming shell that returns 99 for an invalid condition
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text asks only for a non-zero status when a condition is invalid (XCU 2.15 trap, EXIT STATUS).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	command trap "$@"
	pcm_r=$?
	if [ "$pcm_r" -ne 0 ]; then return 99; fi
	return 0
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
