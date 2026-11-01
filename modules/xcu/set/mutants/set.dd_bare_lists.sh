# set -- with no operands lists the variables
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set --$/set >\/dev\/null/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
