# set -u has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -u$//
s/^set -o nounset$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
