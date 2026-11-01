# set -o notify is rejected
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -o notify$/set -Z/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
