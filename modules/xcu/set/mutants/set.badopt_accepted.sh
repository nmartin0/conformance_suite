# an invalid option to set is silently accepted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set [-+]Z$/:/
s/^set [-+]o nosuchoption_pc$/:/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
