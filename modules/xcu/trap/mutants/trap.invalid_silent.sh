# an invalid condition gives no warning
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The warning on standard error is discarded.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() { command trap "$@" 2>/dev/null; }
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
