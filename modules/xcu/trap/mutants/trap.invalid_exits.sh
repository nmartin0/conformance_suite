# an invalid condition makes the shell exit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# When trap fails the shell is ended.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() { command trap "$@" || command exit 2; }
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
