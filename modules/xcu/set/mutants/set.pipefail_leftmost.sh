# under pipefail the status of the leftmost failing command is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_OSED='s/^b=2$/b=3/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
