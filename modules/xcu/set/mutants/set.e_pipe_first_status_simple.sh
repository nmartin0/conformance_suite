# the status of a pipeline is that of its first command (simple command case)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^false | true$/{ set +e; false; echo $? >pc.st; } | true; read -r pcm_s <pc.st; exit "$pcm_s"/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
