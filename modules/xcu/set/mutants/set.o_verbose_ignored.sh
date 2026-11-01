# set -o verbose has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -o verbose$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
