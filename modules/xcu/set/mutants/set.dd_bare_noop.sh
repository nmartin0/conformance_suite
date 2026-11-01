# set -- with no operands changes nothing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set --$/:/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
