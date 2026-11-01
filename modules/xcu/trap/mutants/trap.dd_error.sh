# trap -- is refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The word -- before the operands is not accepted: a diagnostic and status 1.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	if [ -n "$pcm_dd" ]; then echo "trap: bad option --" >&2; return 1; fi
	command trap "$@"
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
