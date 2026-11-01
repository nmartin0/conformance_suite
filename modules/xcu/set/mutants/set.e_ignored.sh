# set -e and set -o errexit have no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -e$//
s/^set -o errexit$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
