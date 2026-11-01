# under pipefail a failing pipeline always has status 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_OSED='s/^b=2$/b=1/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
