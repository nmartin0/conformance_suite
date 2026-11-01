# set +a does not end the effect of set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set +a$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
