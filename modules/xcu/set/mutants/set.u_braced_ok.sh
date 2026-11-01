# an unset variable in braces is not an error under -u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/\${PCU1}/${PCU1-}/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
