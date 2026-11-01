# set -v writes the command after expansion, as a trace does
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -v$/set -x/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
