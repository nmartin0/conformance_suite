# set -m is rejected
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -m$/set -Z/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
