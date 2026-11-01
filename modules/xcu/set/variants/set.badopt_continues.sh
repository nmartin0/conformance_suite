# conforming: the shell does not exit for the error and the status is non-zero
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set \([-+]Z\)$/(set \1)/
s/^set \([-+]o nosuchoption_pc\)$/(set \1)/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
