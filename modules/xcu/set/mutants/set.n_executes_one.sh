# the first command after set -n is still executed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='/^set -n$/{
N
s/^set -n\n\(.*\)$/\1\
set -n/
}'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
