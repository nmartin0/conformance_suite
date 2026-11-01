# an explicit exit command skips the EXIT action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# When the script ends with the exit command, the EXIT trap is dropped first.
cat >pc-mut.part <<'PCM_EOF'
pcm_exit() { command trap - EXIT; command exit "$@"; }
alias exit=pcm_exit
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
