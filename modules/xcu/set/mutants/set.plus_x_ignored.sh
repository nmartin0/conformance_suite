# set +x does not turn the option off
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set +x$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
