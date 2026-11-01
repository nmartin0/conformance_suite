# an invalid condition gives status zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The exit status of trap is always zero.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() { command trap "$@"; return 0; }
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
