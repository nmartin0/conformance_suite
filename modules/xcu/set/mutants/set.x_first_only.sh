# only the first command after set -x is traced
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='/^set -x$/{
N
s/^set -x\n\(.*\)$/set -x\
\1\
set +x/
}'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
