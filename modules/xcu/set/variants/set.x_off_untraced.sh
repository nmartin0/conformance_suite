# conforming: the command that turns tracing off is not itself traced
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_ESED='/set +x/d
/set +o xtrace/d'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
