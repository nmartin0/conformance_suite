# the warning for an invalid condition goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The diagnostic is written to standard output.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() { command trap "$@" 2>&1; }
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
