# the failure of the last command of an OR list does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^false || false$/false || false || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
