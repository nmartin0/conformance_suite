# after set, -- is not a delimiter: operands starting with - or + are options
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -- /set /'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
