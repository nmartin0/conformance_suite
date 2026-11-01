# an invalid option to set gives a message but a zero status, and the shell goes on
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set \([-+]Z\)$/(set \1); true/
s/^set \([-+]o nosuchoption_pc\)$/(set \1); true/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
