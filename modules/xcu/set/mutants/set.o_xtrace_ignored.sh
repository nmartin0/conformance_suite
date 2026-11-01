# set -o xtrace has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -o xtrace$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
