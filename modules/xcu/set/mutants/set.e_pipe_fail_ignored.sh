# a failing pipeline does not end the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^true | false$/true | false || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
