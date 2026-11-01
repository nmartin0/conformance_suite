# the status of a pipeline is that of its first command (subshell case)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^(false; echo one) | cat$/{ set +e; (set -e; false; echo one); echo $? >pc.st; } | cat; read -r pcm_s <pc.st; exit "$pcm_s"/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
