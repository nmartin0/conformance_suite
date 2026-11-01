# set +f does not turn pathname expansion back on
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set +f$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
