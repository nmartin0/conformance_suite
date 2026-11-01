# only the first command after set -v is echoed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='/^set -v$/{
N
s/^set -v\n\(.*\)$/set -v\
\1\
set +v/
}'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
