# the failure of the last command of an AND list does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^true && false$/true \&\& false || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
